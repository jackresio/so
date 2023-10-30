#include "map.h"
#include "stdlib.h"
#include "config.h"

void rand_position(Point *position){
    position->x = ((double)rand()/(double)(RAND_MAX)) * SO_LATO;
    position->y = ((double)rand()/(double)(RAND_MAX)) * SO_LATO;
}

void get_port_position(Point *position){
    static int i = 1;
    switch (i) {
        case 0:
            rand_position(position);
            break;
        case 1:
            position->x = 0;
            position->y = 0;
            i++;
            break;
        case 2:
            position->x = SO_LATO;
            position->y = 0;
            i++;
            break;
        case 3:
            position->x = 0;
            position->y = SO_LATO;
            i++;
            break;
        case 4:
            position->y = SO_LATO;
            position->x = SO_LATO;
            i = 0;
            break;
        default:
            break;
    }
}

int compare_position(Point p1, Point p2){
    if(p1.x == p2.x && p1.y == p2.y)
        return 1;
    else
        return 0;
}
