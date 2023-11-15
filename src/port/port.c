#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <signal.h>

#include "port.h"
#include "../common/entities.h"
#include "../common/sync/sync.h"
#include "../common/memory/memory.h"
#include "../common/utils/map.h"
#include "../common/utils/config.h"
#include "../common/message/message.h"



Port *ports;
Ship *ships;
Good *goods;
int my_index;
long *utils;
int flag;
sigset_t mask;


int main(int argc, char *argv[]) {
    struct sigaction sa;
    char *ptr;
    flag = 0;

    srand(getpid());

    sigemptyset(&mask);
    sigaddset(&mask, SIGALRM);


    bzero(&sa, sizeof(sa));
    sa.sa_handler = sig_handler;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGALRM, &sa, NULL);


    init_config("../config.txt");
    my_index = (int) strtol(argv[1], &ptr, 10);

    set_exec_name("Port");

    shm_get_ports(&ports);
    shm_get_utils(&utils);
    shm_get_goods(&goods);

    sem_sync_get();
    sem_utils_get();
    sem_dock_get();
    sem_dock_set(my_index, ports[my_index].docks);
    sem_goods_get();
    msg_get();

    good_gen();

    sem_sync_ready(ALL);
    sem_sync_wait(ALL);

    while (1) {
        dispatch();
    }

    return 0;
}

void good_gen(void) {
    float normalized_ton;
    int rand_good, rand_ton;

    if (ports[my_index].supply.lot == 0 || ports[my_index].supply.available == false) {
        do {
            rand_good = rand() % SO_MERCI;
        } while (ports[my_index].demand.available == true && rand_good == ports[my_index].demand.index_good);

        rand_ton = rand() % 100;

        sem_utils_reserve(GOODS_SUPPLY);
        utils[GOODS_SUPPLY] += rand_ton;
        sem_utils_release(GOODS_SUPPLY);
        sem_sync_ready(GEN_SUPPLY);
        sem_sync_wait(GEN_SUPPLY);

        normalized_ton = (float) rand_ton / (float) utils[GOODS_SUPPLY];
        normalized_ton = normalized_ton * (float) SO_FILL;
        ports[my_index].supply.lot = (int) nearbyintf(normalized_ton / (float) goods[rand_good].weight);

        if (ports[my_index].supply.lot != 0) {
            ports[my_index].supply.index_good = rand_good;
            ports[my_index].supply.ttl = goods[rand_good].ttl;
            ports[my_index].supply.available = true;
            sem_goods_reserve(rand_good);
            goods[rand_good].available += ports[my_index].supply.lot;
            goods[rand_good].total += ports[my_index].supply.lot;
            sem_goods_release(rand_good);
        }
    } else {
        sem_sync_ready(GEN_SUPPLY);
    }
    if (ports[my_index].demand.lot == 0 || ports[my_index].demand.available == false) {
        do {
            rand_good = rand() % SO_MERCI;
        } while (rand_good == ports[my_index].supply.index_good);

        rand_ton = rand() / (SO_PORTI * 100);

        sem_utils_reserve(GOODS_DEMAND);
        utils[GOODS_DEMAND] += rand_ton;
        sem_utils_release(GOODS_DEMAND);
        sem_sync_ready(GEN_DEMAND);
        sem_sync_wait(GEN_DEMAND);

        normalized_ton = (float) rand_ton / utils[GOODS_DEMAND];
        normalized_ton = normalized_ton * (float) SO_FILL;
        ports[my_index].demand.lot = (int) nearbyintf(normalized_ton / (float) goods[rand_good].weight);
        if (ports[my_index].demand.lot != 0) {
            ports[my_index].demand.index_good = rand_good;
        }
        ports[my_index].demand.available = true;
    } else {
        sem_sync_ready(GEN_DEMAND);
    }
}

void dispatch(void) {
    Msg request, response;
    request = msg_receive(ports[my_index].id);

    switch (request.body.operation) {
        case REQUEST_LOAD:
            response.body.cargo = ports[my_index].supply;
            response.body.operation = REQUEST_LOAD;
            response.mtype = request.body.sender;
            response.body.sender = ports[my_index].id;
            if (request.body.cargo.lot > ports[my_index].supply.lot) {
                response.body.cargo.lot = ports[my_index].supply.lot;
            } else {
                response.body.cargo.lot = request.body.cargo.lot;
            }
            msg_send(response);
            sem_goods_reserve(response.body.cargo.index_good);
            goods[response.body.cargo.index_good].on_ship += response.body.cargo.lot;
            goods[response.body.cargo.index_good].available -= response.body.cargo.lot;
            sem_goods_release(response.body.cargo.index_good);
            ports[my_index].supply.lot -= response.body.cargo.lot;
            ports[my_index].send += response.body.cargo.lot;
            break;
        case REQUEST_ULOAD:
            response.body.cargo = ports[my_index].demand;
            response.body.operation = REQUEST_ULOAD;
            response.mtype = request.body.sender;
            response.body.sender = ports[my_index].id;
            if (request.body.cargo.lot > ports[my_index].demand.lot) {
                response.body.cargo.lot = ports[my_index].demand.lot;
            } else {
                response.body.cargo.lot = request.body.cargo.lot;
            }
            msg_send(response);
            sem_goods_reserve(response.body.cargo.index_good);
            goods[response.body.cargo.index_good].on_ship -= response.body.cargo.lot;
            goods[response.body.cargo.index_good].delivered += response.body.cargo.lot;
            sem_goods_release(response.body.cargo.index_good);
            ports[my_index].demand.lot -= response.body.cargo.lot;
            ports[my_index].received += response.body.cargo.lot;
            break;
    }
}

void sig_handler(int signum) {
    unsigned int id_good = ports[my_index].supply.index_good;
    switch (signum) {
        case SIGALRM:
            if (ports[my_index].supply.available == true && ports[my_index].supply.ttl > 0) {
                ports[my_index].supply.ttl--;
                if (ports[my_index].supply.ttl == 0) {
                    ports[my_index].supply.available = false;
                    goods[id_good].expired_port += ports[my_index].supply.lot;
                    goods[id_good].available -= ports[my_index].supply.lot;
                }
            }
            sem_sync_ready(ALL);
            sem_sync_wait(ALL);
    }
}
