#ifndef __unp_writen_h
#define __unp_writen_h

#include <unistd.h>
#include "error.h"

ssize_t     writen(int, const void *, size_t);
void        Writen(int, void *, size_t);

#endif

