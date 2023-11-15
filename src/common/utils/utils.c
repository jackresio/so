#include "utils.h"
#include <execinfo.h>

int error_value;
char * exec_name;

void check_error(char * file, int line){

    fprintf(stderr,"%s:%d: %s PID=%5d: Error %d (%s)\n", file, line, exec_name, getpid(), errno,strerror(errno));
    errno = 0;
}

void set_exec_name(char * name){
    exec_name = name;
}


