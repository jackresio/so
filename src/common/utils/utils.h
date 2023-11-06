#ifndef SISTEMI_OPERATIVI_UTILS_H
#define SISTEMI_OPERATIVI_UTILS_H

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * Macro that print on stderr, file, line, pid, error code
 * if errno is set
 */
#define CHECK_ERROR if (errno) {fprintf(stderr,				\
				       "%s:%d: PID=%5d: Error %d (%s)\n", \
				       __FILE__,			\
				       __LINE__,			\
				       getpid(),			\
				       errno,				\
				       strerror(errno));    \
                       exit(0);}

#endif
