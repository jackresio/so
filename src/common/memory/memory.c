#include "memory.h"
#include "../constants.h"

int ships_id = 0;
int ports_id = 0;
int goods_id = 0;
int utils_id = 0;

void shm_init(void){
    int ship_ipc;
    int port_ipc;
    int goods_ipc;
    int utils_ipc;

    ship_ipc = ftok(KEY_MASTER, KEY_SHIPS);
    if (ship_ipc == - 1)
        check_error(__FILE__, __LINE__);

    port_ipc = ftok(KEY_MASTER, KEY_PORTS);
    if (port_ipc == - 1)
        check_error(__FILE__, __LINE__);

    goods_ipc = ftok(KEY_MASTER, KEY_GOODS);
    if (goods_ipc == - 1)
        check_error(__FILE__, __LINE__);
    utils_ipc = ftok(KEY_MASTER, KEY_UTILS);
    if (utils_ipc == - 1)
        check_error(__FILE__, __LINE__);

    ships_id = shmget(ship_ipc, sizeof(Ship) * SO_NAVI, IPC_CREAT | 0777 );
    if (ships_id == -1)
        check_error(__FILE__, __LINE__);
    ports_id = shmget(port_ipc, sizeof(Port) * SO_PORTI, IPC_CREAT | 0777 );
    if (ports_id == -1)
        check_error(__FILE__, __LINE__);
    goods_id = shmget(goods_ipc, sizeof(Good) * SO_MERCI, IPC_CREAT | 0777 );
    if (goods_id == -1)
        check_error(__FILE__, __LINE__);
    utils_id = shmget(utils_ipc, sizeof (long) * UTILS_LENGTH, IPC_CREAT | 0777 );
    if (utils_id == -1)
        check_error(__FILE__, __LINE__);
}

void shm_get_ships(Ship **ships){
    int ships_ipc;

    ships_ipc = ftok(KEY_MASTER, KEY_SHIPS);
    if (ships_ipc == -1)
        check_error(__FILE__, __LINE__);

    ships_id = shmget(ships_ipc, sizeof(Ship) * SO_NAVI,  0777);
    if (ships_id == -1)
        check_error(__FILE__, __LINE__);

    *ships = shmat(ships_id, NULL, 0);
    if (ships == (void *) -1)
        check_error(__FILE__, __LINE__);
}

void shm_get_ports(Port **ports){
    int ports_ipc;

    ports_ipc = ftok(KEY_MASTER, KEY_PORTS);
    if (ports_ipc == -1)
        check_error(__FILE__, __LINE__);

    ports_id = shmget(ports_ipc, sizeof(Port) * SO_PORTI, 0777);
    if (ports_id == -1)
        check_error(__FILE__, __LINE__);

    *ports = shmat(ports_id, NULL, 0);
    if (ports == (void *) -1)
        check_error(__FILE__, __LINE__);
}

void shm_get_goods(Good **goods){
    int goods_ipc;

    goods_ipc = ftok(KEY_MASTER, KEY_GOODS);
    if (goods_ipc == -1)
        check_error(__FILE__, __LINE__);

    goods_id = shmget(goods_ipc, sizeof(Good) * SO_MERCI, 0777);
    if (goods_id == -1)
        check_error(__FILE__, __LINE__);

    *goods = shmat(goods_id, NULL, 0);
    if (goods == (void *) -1)
        check_error(__FILE__, __LINE__);
}

void shm_get_utils(long **utils){
    int utils_ipc;

    utils_ipc = ftok(KEY_MASTER, KEY_UTILS);
    if (utils_ipc == -1)
        check_error(__FILE__, __LINE__);

    utils_id = shmget(utils_ipc, sizeof(int) * 10, 0777);
    if (utils_id == -1)
        check_error(__FILE__, __LINE__);

    *utils = shmat(utils_id, NULL, 0);
    if (utils == (void *) -1)
        check_error(__FILE__, __LINE__);
}

void shm_free(void){
    int error;
    error = shmctl(ships_id, IPC_RMID, NULL);
    if (error == -1)
        check_error(__FILE__, __LINE__);

    error = shmctl(ports_id, IPC_RMID, NULL);
    if (error == -1)
        check_error(__FILE__, __LINE__);

    error = shmctl(goods_id, IPC_RMID, NULL);
    if (error == -1)
        check_error(__FILE__, __LINE__);

    error = shmctl(utils_id, IPC_RMID, NULL);
    if (error == -1)
        check_error(__FILE__, __LINE__);
}
