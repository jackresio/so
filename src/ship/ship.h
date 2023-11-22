#ifndef SISTEMIOPERATIVI_SHIP_H
#define SISTEMIOPERATIVI_SHIP_H

#include "../common/entities.h"

void sig_handler(int signum);

void move(Port *dest);

void load(int port_index);

void unload(int port_index);

int nearest_port(int last_port);

#endif
