#ifndef SISTEMI_OPERATIVI_MEMORY_H
#define SISTEMI_OPERATIVI_MEMORY_H

#include <sys/shm.h>
#include "../utils/utils.h"
#include "../utils/config.h"
#include "../entities.h"

/**
 * Initialize all the shared memory segmented used in the simulation
 * This function should be called once in the main process
 */
void shm_init();

/**
 * Assign to ships the Ship shared memory segment
 * @param ships
 */
void shm_get_ships(Ship **ships);

/**
 * Assign to ports the Port shared memory segment
 * @param ports
 */
void shm_get_ports(Port **ports);

/**
 * Assign to goods the Good shared memory segment
 * @param goods
 */
void shm_get_goods(Good **goods);

/**
 * Assign to utils the Utils shared memory segment
 * @param utils
 */
void shm_get_utils(long **utils);

void shm_free(void );



#endif
