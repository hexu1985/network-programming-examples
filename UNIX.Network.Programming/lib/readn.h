#ifndef __unp_readn_h
#define __unp_readn_h

#include <unistd.h>
#include "error.h"

ssize_t	readn(int, void *, size_t);
ssize_t Readn(int, void *, size_t);

#endif

