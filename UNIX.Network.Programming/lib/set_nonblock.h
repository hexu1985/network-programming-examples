#ifndef __unp_set_nonblock_h
#define __unp_set_nonblock_h

#include <fcntl.h>
#include "error.h"

int set_nonblock(int fd);
void Set_nonblock(int fd);

#endif
