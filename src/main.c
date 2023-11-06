#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "common/entities.h"
#include "common/utils/config.h"
#include "common/memory/memory.h"

Ship *ships;
Port *ports;
Good *goods;
long *utils;


int main(void){
    int i = 0;
    char **child_argv;

    printf("Start Simulation \n");

    srand(time(0));

    child_argv = calloc(3, sizeof(char *));
    for (i = 0; i < 2; i++){
        child_argv[i] = calloc(16, sizeof(char));
    }
    child_argv[2] = NULL;

    printf("Reading configuration file \n");
    init_config("../config.txt");

    printf("Allocating memory \n");
    shm_init();
    shm_get_ships(&ships);
    shm_get_ports(&ports);
    shm_get_goods(&goods);
    shm_get_utils(&utils);

    printf("Initialize semaphore \n");


    return 0;



}
