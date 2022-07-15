#ifndef __unp_writen_h
#define __unp_writen_h

#include <unistd.h>
#include "error.h"

ssize_t writen(int fd, const void *vptr, size_t n);
void Writen(int fd, void *ptr, size_t nbytes);

#endif

