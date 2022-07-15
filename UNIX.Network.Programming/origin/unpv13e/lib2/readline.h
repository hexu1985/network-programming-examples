#ifndef __unp_readline_h
#define __unp_readline_h

#include <unistd.h>
#include "error.h"

ssize_t readline(int fd, void *vptr, size_t maxlen);
ssize_t Readline(int fd, void *ptr, size_t maxlen);

#endif


