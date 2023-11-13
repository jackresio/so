/**
 * Synchronization library
 */
#ifndef SISTEMI_OPERATIVI_SYNC_H
#define SISTEMI_OPERATIVI_SYNC_H

typedef enum {
    GOODS_SUPPLY,
    GOODS_DEMAND,
    TOTAL_SUPPLY,
    TOTAL_DEMAND
} SemUtils;

/**
 * ALL: semaphore for sync all process
 * GEN_SUPPLY: semaphore for sync port when creating supply
 * GEN_DEMAND: semaphore for sync port when creating demand
 */
typedef enum {
    ALL,
    GEN_SUPPLY,
    GEN_DEMAND
} SemSync;

/**
 * Initialize all the semaphore needed by the process
 */
void sem_init(void);

int sem_sync_get(void);

int sem_utils_get(void);

int sem_goods_get(void);

int sem_dock_get(void);

void sem_sync_wait(SemSync sem_num);

void sem_sync_ready(SemSync sem_num);

int sem_sync_reset(SemSync sem_num);

void sem_utils_reserve(SemUtils sem_num);

void sem_utils_release(SemUtils sem_num);

void sem_goods_reserve(int sem_num);

void sem_goods_release(int sem_num);

int sem_dock_set(int port_index, unsigned int docks);

int dock_reserve(int port_index);

int dock_release(int port_index);

void sem_close(void);

int sem_sync_info(int sem_num);

#endif
