#ifndef __unp_readn_h
#define __unp_readn_h

#include <unistd.h>
#include "error.h"

ssize_t	readn(int fd, void *vptr, size_t n);
ssize_t Readn(int fd, void *ptr, size_t nbytes);

#endif

