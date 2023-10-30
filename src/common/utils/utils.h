#ifndef SISTEMI_OPERATIVI_UTILS_H
#define SISTEMI_OPERATIVI_UTILS_H

#include <errno.h>

/**
 * Macro that print on stderr, file, line, pid, error code
 * if errno is set
 */
#define TEST_ERROR if (errno) {fprintf(stderr,				\
				       "%s:%d: PID=%5d: Error %d (%s)\n", \
				       __FILE__,			\
				       __LINE__,			\
				       getpid(),			\
				       errno,				\
				       strerror(errno));}

#endif
