#include "memory.h"

#define UTILS_LENGTH 5


int ships_id = 0;
int ports_id = 0;
int goods_id = 0;
int utils_id = 0;

void shm_init(void){
    int ship_ipc, port_ipc, goods_ipc, utils_ipc;

    ship_ipc = ftok(KEY_MASTER, KEY_SHIPS);
    port_ipc = ftok(KEY_MASTER, KEY_PORTS);
    goods_ipc = ftok(KEY_MASTER, KEY_GOODS);
    utils_ipc = ftok(KEY_MASTER, KEY_UTILS);
    CHECK_ERROR

    shmget(ship_ipc, sizeof(Ship) * SO_NAVI, IPC_CREAT | 0777 );
    shmget(port_ipc, sizeof(Port) * SO_PORTI, IPC_CREAT | 0777 );
    shmget(goods_ipc, sizeof(Good) * SO_MERCI, IPC_CREAT | 0777 );
    shmget(utils_ipc, sizeof (long) * UTILS_LENGTH, IPC_CREAT | 0777 );
    CHECK_ERROR

    return;
}

void shm_get_ships(Ship **ships){
    int ships_ipc;

    ships_ipc = ftok(KEY_MASTER, KEY_SHIPS);
    CHECK_ERROR

    ships_id = shmget(ships_ipc, sizeof(Ship) * SO_NAVI,  0777);
    *ships = shmat(ships_id, NULL, 0);
    CHECK_ERROR
}

void shm_get_ports(Port **ports){
    int ports_ipc;

    ports_ipc = ftok(KEY_MASTER, KEY_PORTS);
    CHECK_ERROR

    ports_id = shmget(ports_ipc, sizeof(Port) * SO_PORTI, 0777);
    *ports = shmat(ports_id, NULL, 0);
    CHECK_ERROR
}

void shm_get_goods(Good **goods){
    int goods_ipc;

    goods_ipc = ftok(KEY_MASTER, KEY_GOODS);
    CHECK_ERROR

    goods_id = shmget(goods_ipc, sizeof(Good) * SO_MERCI, 0777);
    *goods = shmat(goods_id, NULL, 0);
    CHECK_ERROR
}

void shm_get_utils(long **utils){
    int utils_ipc;

    utils_ipc = ftok(KEY_MASTER, KEY_UTILS);
    CHECK_ERROR

    utils_id = shmget(utils_ipc, sizeof(int) * 10, 0777);
    *utils = shmat(utils_id, NULL, 0);
    CHECK_ERROR
}

void shm_free(void){
    shmctl(ships_id, IPC_RMID, NULL);
    shmctl(ports_id, IPC_RMID, NULL);
    shmctl(goods_id, IPC_RMID, NULL);
    shmctl(utils_id, IPC_RMID, NULL);
}