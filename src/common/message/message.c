#include "message.h"

#include "../constants.h"
#include "../utils/utils.h"

#include <sys/msg.h>

int msg_id;

int msg_init(void){
    msg_id = msgget(ftok(KEY_MASTER, KEY_MESSAGE),IPC_CREAT | IPC_EXCL | 0777);
    if (msg_id == -1)
        check_error();
    return msg_id;
}

void msg_get(void){
    msg_id = msgget(ftok(KEY_MASTER, KEY_MESSAGE),IPC_EXCL | 0777);
    if (msg_id == -1)
        check_error();
}

void msg_send(Msg msg){
     error_value = msgsnd(msg_id, &msg, sizeof(MsgBody), 0);
    if (error_value == -1)
        check_error();
}

Msg msg_receive(long type){
    Msg msg;
    error_value = msgrcv(msg_id, &msg, sizeof(MsgBody), type, 0);
    if (error_value == -1)
        check_error();
    return msg;
}

void msg_close(void ){
    error_value = msgctl(msg_id, IPC_RMID, NULL);
    if (error_value == -1)
        check_error();
}
