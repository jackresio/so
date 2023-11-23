#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <signal.h>

#include "common/entities.h"
#include "common/utils/config.h"
#include "common/memory/memory.h"
#include "common/sync/sync.h"
#include "common/utils/map.h"
#include "main.h"
#include "common/message/message.h"

Ship *ships;
Port *ports;
Good *goods;
long *utils;


int main(int argc, char *argv[]){
    int i;
    unsigned int goods_supply_global = 0, goods_demand_global = 0;
    pid_t pid, pgid;
    unsigned int loaded = 0, empty = 0, op = 0, moving = 0;
    int stop = false;
    TermStatus termination_status;
    unsigned int days = 0;
    char **child_argv;
    struct sigaction sa;

    printf("Start simulation \n");

    set_exec_name("Main");

    srand(time(0));
    bzero(&sa, sizeof(sa));
    sa.sa_handler = sig_handler;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGALRM, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);



    pgid = getpgid(getpid());

    child_argv = calloc(3, sizeof(char *));
    for (i = 0; i < 2; i++){
        child_argv[i] = calloc(16, sizeof(char));
    }
    child_argv[2] = NULL;

    printf("Reading configuration file \n");
    init_config("../config.txt");

    printf("Allocating memory \n");
    shm_init();
    if (SO_NAVI > 0) {
        shm_get_ships(&ships);
    }
    shm_get_ports(&ports);
    shm_get_goods(&goods);
    shm_get_utils(&utils);
    msg_init();

    printf("Initialize semaphore \n");
    sem_init();

    printf("Generating goods \n");
    for (i = 0; i < SO_MERCI; i++) {
        goods[i].id = i + 1;
        goods[i].ttl = SO_MIN_VITA + (rand() % (SO_MAX_VITA - SO_MIN_VITA + 1));
        goods[i].weight = 1 + (rand() % SO_SIZE);
    }

    for (i = 0; i < SO_MERCI; i++) {
        printf("GOOD ID: ");
        printf("%d, weight: %d, ttl: %d \n", goods[i].id, goods[i].weight, goods[i].ttl);
    }

    printf("Creating ports \n");
    strcpy(child_argv[0], "port");
    for (i = 0; i < SO_PORTI; i++) {
        sprintf(child_argv[1], "%d", i);
        get_port_position(&ports[i].position);
        ports[i].docks = rand() % SO_BANCHINE + 1;
        ports[i].supply.available = false;
        ports[i].demand.available = false;
        pid = fork();
        switch (pid) {
            case 0:
                setpgid(getpid(), pgid);
                execv("port.bin", child_argv);
                break;
            case -1:
                check_error(__FILE__, __LINE__);
                break;
            default:
                ports[i].id = pid;
                break;
        }
    }

    printf("Creating ships \n");
    strcpy(child_argv[0], "ship");
    for (i = 0; i < SO_NAVI; i++) {
        sprintf(child_argv[1], "%d", i);
        rand_position(&ships[i].position);
        ships[i].capacity = SO_CAPACITY;
        ships[i].speed = SO_SPEED;
        ships[i].status = EMPTY;
        pid = fork();
        switch (pid) {
            case 0:
                setpgid(getpid(), pgid);
                execv("ship.bin", child_argv);
                break;
            case -1:
                check_error(__FILE__, __LINE__);
                break;
            default:
                ships[i].id = pid;
                break;
        }
    }

    sem_sync_ready(ALL);
    sem_sync_wait(ALL);
    sem_sync_reset(ALL);
    sem_sync_reset(GEN_DEMAND);
    sem_sync_reset(GEN_SUPPLY);

    printf("Start simulation\n");

    while (!stop) {
        alarm(1);
        pause();
        kill(0, SIGALRM);
        printf("\nGiorno %d \n", days + 1);

        /*Print goods*/
        printf("GOODS INFORMATION\n");
        for (i = 0; i < SO_MERCI; i++) {
            printf("ID: %d ", goods[i].id);
            printf("IN PORT: %d ", goods[i].available);
            printf("ON SHIP: %d ", goods[i].on_ship);
            printf("DELIVERED: %d ", goods[i].delivered);
            printf("EXPIRED PORT: %d ", goods[i].expired_port);
            printf("EXPIRED SHIP: %d ", goods[i].expired_ship);
            printf("TOTAL: %d \n", goods[i].total);
        }

        /*Print ships*/
        op = 0;
        empty = 0;
        loaded = 0;
        moving = 0;
        for (i = 0; i < SO_NAVI; i++) {
            switch (ships[i].status) {
                case OP:
                    op++;
                    break;
                case EMPTY:
                    empty++;
                    break;
                case LOADED:
                    loaded++;
                    break;
                case MOVING:
                    moving++;
                    break;
            }
        }
        printf("SHIPS INFORMATION\n");
        printf("LOADED: %d \n", loaded);
        printf("EMPTY: %d \n", empty);
        printf("OPERATION: %d \n", op);
        printf("MOVING: %d \n", moving);

        /*Print ports*/
        goods_demand_global = 0;
        goods_supply_global = 0;
        printf("PORTS INFORMATION\n");
        for (i = 0; i < SO_PORTI; i++) {
            printf("ID: %d ", ports[i].id);
            printf("AVAILABLE %d (lot): %d ", ports[i].supply.index_good, ports[i].supply.lot);
            printf("SEND (lot): %d ", ports[i].send);
            printf("RECEIVED %d (lot): %d \n", ports[i].demand.index_good, ports[i].received);
            goods_demand_global += ports[i].demand.lot;
            goods_supply_global += ports[i].supply.lot;
        }
        fflush(stdout);

        if (days >= SO_DAYS) {
            stop = true;
            termination_status = DAYS;
        }
        if (goods_supply_global == 0) {
            stop = true;
            termination_status = SUPPLY_END;
        }
        if (goods_demand_global == 0) {
            stop = true;
            termination_status = DEMAND_END;
        }
        if (!stop){
            sem_sync_ready(ALL);
            sem_sync_wait(ALL);
            sem_sync_reset(ALL);
            days++;
        }
    }

    for (i = 0; i < SO_NAVI; i++)
        kill(ships[i].id, SIGTERM);
    for (i = 0; i < SO_PORTI; i++)
        kill(ports[i].id, SIGTERM);

    /*Final Report*/
    printf("Final report \n");
    printf("%d", termination_status);


    return 0;
}


void free_memory(void) {
    shmdt(ships);
    shmdt(ports);
    shmdt(goods);
    shmdt(utils);
    sem_close();
    shm_free();
    msg_close();
}

void sig_handler(int sig_num) {
    switch (sig_num) {
        case SIGALRM:
            break;
        case SIGSTOP:
        case SIGINT:
        case SIGTERM:
            free_memory();
            exit(0);
    }
}
