/**
 * Library for process sync
 */

#include <sys/sem.h>
#include "sync.h"
#include "../constants.h"
#include "../utils/utils.h"
#include "../utils/config.h"

int sem_sync;
int sem_utils;
int sem_goods;
int sem_dock;

void sem_init(void){
    int i;

    sem_sync = semget(ftok(KEY_MASTER, KEY_SYNC), 3, IPC_CREAT | IPC_EXCL | 0777);
    error_value = semctl(sem_sync, ALL, SETVAL, SO_NAVI + SO_PORTI + WEATHER + 1);
    if (error_value == -1)
        check_error(__FILE__, __LINE__);
    error_value = semctl(sem_sync, GEN_SUPPLY, SETVAL, SO_PORTI);
    if (error_value == -1)
        check_error(__FILE__, __LINE__);
    error_value = semctl(sem_sync, GEN_DEMAND, SETVAL, SO_PORTI);
    if (error_value == -1)
        check_error(__FILE__, __LINE__);

    sem_utils = semget(ftok(KEY_MASTER, KEY_UTILS), 10, IPC_CREAT | IPC_EXCL | 0777);
    if (sem_utils == -1)
        check_error(__FILE__, __LINE__);
    for (i = 0; i < UTILS_LENGTH; ++i) {
        error_value = semctl(sem_utils, i, SETVAL, 1);
        if (error_value == -1)
            check_error(__FILE__, __LINE__);
    }

    sem_goods = semget(ftok(KEY_MASTER, KEY_SEMGOODS), SO_MERCI, IPC_CREAT | IPC_EXCL | 0777);
    if (sem_goods == -1)
        check_error(__FILE__, __LINE__);
    for (i = 0; i < SO_MERCI; ++i) {
        error_value = semctl(sem_utils, i, SETVAL, 1);
        if (error_value == -1)
            check_error(__FILE__, __LINE__);
    }

    sem_dock = semget(ftok(KEY_MASTER, KEY_DOCK), SO_PORTI, IPC_CREAT | IPC_EXCL | 0777);
    if (sem_dock == -1)
        check_error(__FILE__, __LINE__);
}

int sem_sync_get(void){
    sem_sync = semget(ftok(KEY_MASTER, KEY_SYNC), 3, 0777);
    if (sem_sync == -1)
        check_error(__FILE__, __LINE__);
    return sem_sync;
}

int sem_utils_get(void){
    sem_utils = semget(ftok(KEY_MASTER, KEY_UTILS), 10, 0777);
    if (sem_utils == -1)
        check_error(__FILE__, __LINE__);
    return sem_utils;
}

int sem_goods_get(void){
    sem_goods = semget(ftok(KEY_MASTER, KEY_SEMGOODS), SO_MERCI, 0777);
    if (sem_goods == -1)
        check_error(__FILE__, __LINE__);
    return sem_goods;
}

int sem_dock_get(void){
    sem_dock = semget(ftok(KEY_MASTER, KEY_DOCK), SO_PORTI, 0777);
    if (sem_dock == -1)
        check_error(__FILE__, __LINE__);
    return sem_dock;
}


int sem_reserve(int sem_id, int sem_num){
    struct sembuf sops;

    sops.sem_num = sem_num;
    sops.sem_op = -1;
    sops.sem_flg = 0;

    error_value = semop(sem_id, &sops, 1);
    if (error_value == -1){
        check_error(__FILE__, __LINE__);
    }

    return error_value;
}

int sem_release(int sem_id, int sem_num){
    struct sembuf sops;

    sops.sem_num = sem_num;
    sops.sem_op = 1;
    sops.sem_flg = 0;

    error_value = semop(sem_id, &sops, 1);
    if (error_value == -1)
        check_error(__FILE__, __LINE__);

    return error_value;
}

int sem_wait(int sem_id, int sem_num){
    struct  sembuf sops;

    sops.sem_num = sem_num;
    sops.sem_op = 0;
    sops.sem_flg = 0;

    error_value = semop(sem_id, &sops, 1);
    if (error_value == -1)
        check_error(__FILE__, __LINE__);

    return error_value;
}

void sem_sync_wait(SemSync sem_num) {
    sem_wait(sem_sync, sem_num);
}

void sem_sync_ready(SemSync sem_num) {
    sem_reserve(sem_sync, sem_num);
}

int sem_sync_reset(SemSync sem_num) {
    switch (sem_num) {
        case ALL:
            error_value = semctl(sem_sync, ALL, SETVAL, SO_NAVI + SO_PORTI + WEATHER + 1);
            if (error_value == -1)
                check_error(__FILE__, __LINE__);
            return error_value;
        case GEN_SUPPLY:
            error_value = semctl(sem_sync, GEN_SUPPLY, SETVAL, SO_PORTI);
            if (error_value == -1)
                check_error(__FILE__, __LINE__);
            return error_value;
        case GEN_DEMAND:
            error_value = semctl(sem_sync, GEN_DEMAND, SETVAL, SO_PORTI);
            if (error_value == -1)
                check_error(__FILE__, __LINE__);
            return error_value;
        default:
            return -1;
    }
}

void sem_utils_reserve(SemUtils sem_num){
    sem_reserve(sem_utils, sem_num);
}

void sem_utils_release(SemUtils sem_num) {
    sem_release(sem_utils, sem_num);
}

void sem_goods_reserve(int sem_num){
    sem_reserve(sem_utils, sem_num);
}

void sem_goods_release(int sem_num){
    sem_release(sem_utils, sem_num);
}

int sem_dock_set(int port_index, unsigned int docks){
    return semctl(sem_dock, port_index, SETVAL, docks);
}

int dock_reserve(int port_index){
    return sem_reserve(sem_dock, port_index);
}

int dock_release(int port_index){
    return sem_release(sem_dock, port_index);
}

void sem_close(void){
    error_value = semctl(sem_sync, 0, IPC_RMID);
    if (error_value == -1)
        check_error(__FILE__, __LINE__);
    error_value = semctl(sem_utils, 0, IPC_RMID);
    if (error_value == -1)
        check_error(__FILE__, __LINE__);
    error_value = semctl(sem_dock, 0, IPC_RMID);
    if (error_value == -1)
        check_error(__FILE__, __LINE__);
    error_value = semctl(sem_goods, 0, IPC_RMID);
    if (error_value == -1)
        check_error(__FILE__, __LINE__);
}

int sem_sync_info(int sem_num){
    return semctl(sem_sync, sem_num, GETVAL);
}






