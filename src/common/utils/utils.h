#ifndef SISTEMI_OPERATIVI_UTILS_H
#define SISTEMI_OPERATIVI_UTILS_H

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

extern int error_value;

/**
 * Macro that print on stderr, file, line, pid, error code
 * if errno is set
 */
void check_error(char * file, int line);

void set_exec_name(char * name);

#endif
