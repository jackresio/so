#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include "ship.h"

#include "../common/utils/utils.h"
#include "../common/memory/memory.h"
#include "../common/message/message.h"
#include "../common/sync/sync.h"


Ship *ships;
Port *ports;
Good *goods;
int my_index;
int flag;
int days;

int main(int argc, char *argv[]) {
    struct sigaction sa;
    char *ptr;
    int dest_index;

    days = 0;

    flag = 0;

    init_config("../config.txt");

    set_exec_name("Ship");

    bzero(&sa, sizeof(sa));
    sa.sa_handler = sig_handler;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGALRM, &sa, NULL);

    shm_get_ships(&ships);
    shm_get_ports(&ports);
    shm_get_goods(&goods);
    sem_sync_get();
    sem_goods_get();
    msg_get();


    sem_sync_ready(ALL);
    sem_sync_wait(ALL);

    my_index = (int) strtol(argv[1], &ptr, 10);

    dest_index = nearest_port();

    while (1) {
        if (dest_index > -1) {
            move(&ports[dest_index]);
            if (ships[my_index].status == EMPTY) {
                dock_reserve(dest_index);
                ships[my_index].status = OP;
                load(dest_index);
                dock_release(dest_index);
            } else if (ships[my_index].status == LOADED) {
                dock_reserve(dest_index);
                ships[my_index].status = OP;
                unload(dest_index);
                dock_release(dest_index);
            }
        } else {
            pause();
        }
        dest_index = nearest_port();
    }

}

int nearest_port(void) {
    int i;
    double travel_time, min_distance = 0, temp_distance;
    int dest = -1;

    switch (ships[my_index].status) {
        case EMPTY:
            for (i = 0; i < SO_PORTI; i++) {
                temp_distance = calc_distance(ships[my_index].position, ports[i].position);
                travel_time = temp_distance / SO_SPEED;
                if (travel_time < ports[i].supply.ttl) {
                    min_distance = temp_distance;
                    dest = i;
                    break;
                }
            }
            while (i < SO_PORTI) {
                temp_distance = calc_distance(ships[my_index].position, ports[i].position);
                travel_time = temp_distance / SO_SPEED;
                if (travel_time < ports[i].supply.ttl && temp_distance < min_distance) {
                    min_distance = temp_distance;
                    dest = i;
                }
                i++;
            }
            break;
        case LOADED:
            for (i = 0; i < SO_PORTI; i++) {
                if (ports[i].demand.index_good == ships[my_index].cargo.index_good) {
                    temp_distance = calc_distance(ships[my_index].position, ports[i].position);
                    travel_time = temp_distance / SO_SPEED;
                    if (travel_time < ships[my_index].cargo.ttl) {
                        min_distance = temp_distance;
                        dest = i;
                        break;
                    }
                }
            }
            while (i < SO_PORTI) {
                if (ports[i].demand.index_good == ships[my_index].cargo.index_good) {
                    temp_distance = calc_distance(ships[my_index].position, ports[i].position);
                    travel_time = temp_distance / SO_SPEED;
                    if (travel_time < ships[my_index].cargo.ttl && temp_distance < min_distance) {
                        min_distance = temp_distance;
                        dest = i;
                    }
                }
                i++;
            }
            break;
        case ALIVE:
        case SUNK:
        case OP:
        case MOVING:;
        case SWELL:
        case STORM:
            break;
    }
    return dest;
}

void move(Port *dest) {
    struct timespec req, rem;
    double distance, travel_time;
    int res;
    distance = calc_distance(ships[my_index].position, dest->position);

    travel_time = distance / SO_SPEED;

    req.tv_sec = (int) travel_time;
    req.tv_nsec = (long) ((travel_time - req.tv_sec) * 1e9);

    res = nanosleep(&req, &rem);
    while (res == -1) {
        res = nanosleep(&rem, &rem);
    }
    ships[my_index].position = dest->position;
}

void operate(Cargo cargo) {
    struct timespec req, rem;
    double op_time;
    int res;
    unsigned int debug = goods[cargo.index_good].weight;

    op_time = ((double) cargo.lot * debug / SO_LOADSPEED);

    req.tv_sec = (int) op_time;
    req.tv_nsec = (long) ((op_time - req.tv_sec) * 1e9);

    res = nanosleep(&req, &rem);
    while (res == -1) {
        res = nanosleep(&rem, &rem);
    }
}

void load(int port_index) {
    Msg request, response;
    int index_good = ports[port_index].supply.index_good;

    request.mtype = ports[port_index].id;
    request.body.cargo = ports[port_index].supply;
    request.body.sender = ships[my_index].id;
    request.body.operation = REQUEST_LOAD;
    request.body.cargo.lot = nearbyintf(((float) ships[my_index].capacity / goods[index_good].weight));
    msg_send(request);
    response = msg_receive(ships[my_index].id);
    ships[my_index].port = port_index;
    operate(response.body.cargo);

    ships[my_index].cargo = response.body.cargo;
    ships[my_index].capacity -= response.body.cargo.lot * goods[index_good].weight;
    ships[my_index].status = LOADED;
}

void unload(int port_index) {
    Msg request, response;
    unsigned int index_good = ports[port_index].demand.index_good;

    request.mtype = ports[port_index].id;
    request.body.cargo = ports[port_index].demand;
    request.body.sender = ships[my_index].id;
    request.body.operation = REQUEST_ULOAD;
    request.body.cargo.lot = (ships[my_index].cargo.lot > ports[port_index].demand.lot) ?
                             ports[port_index].demand.lot : ships[my_index].cargo.lot;
    msg_send(request);
    response = msg_receive(ships[my_index].id);
    ships[my_index].port = port_index;
    operate(response.body.cargo);

    ships[my_index].cargo = response.body.cargo;
    ships[my_index].capacity += response.body.cargo.lot * goods[index_good].weight;
    if (ships[my_index].capacity == SO_CAPACITY) {
        ships[my_index].status = EMPTY;
    } else {
        ships[my_index].status = LOADED;
    }
}


void sig_handler(int signum) {
    switch (signum) {
        case SIGALRM:
            if (ships[my_index].cargo.available == true && ships[my_index].cargo.ttl > 0) {
                ships[my_index].cargo.ttl--;
            }
            if (ships[my_index].cargo.ttl == 0) {
                ships[my_index].cargo.available = false;
                ships[my_index].status = EMPTY;
            }
            sem_sync_ready(ALL);
            sem_sync_wait(ALL);
            break;
    }
}

