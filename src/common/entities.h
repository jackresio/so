#ifndef SISTEMI_OPERATIVI_ENTITIES_H
#define SISTEMI_OPERATIVI_ENTITIES_H

#ifndef SISTEMIOPERATIVI_ENTITIES_H
#define SISTEMIOPERATIVI_ENTITIES_H

#include "utils/map.h"

/**
 * Struttura che rappresenta la merce generata ad inizio simulazione
 * Contiene tutte le informazioni di una specifica merce
 */
struct good {
    unsigned int id;
    unsigned int ttl;
    unsigned int weight;
    unsigned int available;
    unsigned int delivered;
    unsigned int on_ship;
    unsigned int expired_port;
    unsigned int expired_ship;
    unsigned int total;
} typedef Good;

/**
 * Struttura che rappresenta il carico
 * Un carico è composta da una merce, un tempo di vita e quanti lotti
 * di quella merce sono presenti
 */
struct cargo {
    unsigned int available;
    unsigned int id_good;
    unsigned int lot;
    unsigned int ttl;
}typedef Cargo;

/*
 * Rappresentazione di tutti gli stati in cui si può trovare una nave
 */
typedef enum{
    ALIVE,
    SUNK,
    OP,             /*Operation, durante carico e scarico*/
    MOVING,
    EMPTY,
    LOADED,
    SWELL,
    STORM
} ShipState;

struct ship {
    pid_t id;
    Point position;
    Cargo cargo;
    int port;
    unsigned int speed;
    unsigned int capacity;
    ShipState status;
}typedef Ship;

struct port {
    Point position;
    pid_t id;
    unsigned int docks;
    unsigned int send;
    unsigned int received;
    Cargo demand;
    Cargo supply;
} typedef Port;

#endif





#endif
