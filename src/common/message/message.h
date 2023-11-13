#ifndef SISTEMI_OPERATIVI_MESSAGE_H
#define SISTEMI_OPERATIVI_MESSAGE_H

#include "../entities.h"

enum {
    REQUEST_LOAD,
    REQUEST_ULOAD
}typedef RequestType;

/**
 * Il messaggio vero e proprio
 */
struct msg_body {
    unsigned int sender;
    unsigned int receiver;
    RequestType operation;
    Cargo cargo;
} typedef MsgBody;

/**
 * Struttura utilizzata per inviare i messaggi nella coda
 */
struct message {
    long mtype;
    MsgBody body;
} typedef Msg;

int msg_init(void);

void msg_get(void);

void msg_send(Msg msg);

Msg msg_receive(long type);

void msg_close(void );

#endif
