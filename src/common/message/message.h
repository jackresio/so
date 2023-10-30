#ifndef SISTEMI_OPERATIVI_MESSAGE_H
#define SISTEMI_OPERATIVI_MESSAGE_H

#include "../entities.h"

/**
 * Il messaggio vedo e proprio
 */
struct msg_body {
    unsigned int sender;
    unsigned int receiver;
    unsigned int operation;
    Cargo cargo;
} typedef MsgBody;

/**
 * Struttura utilizzata per inviare i messaggi nella coda
 */
struct message {
    long mtype;
    MsgBody body;
} typedef Msg;

#endif
