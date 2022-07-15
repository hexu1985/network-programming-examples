#ifndef __unp_my_addrs_h
#define __unp_my_addrs_h

#include <netdb.h>
#include "error.h"

char **my_addrs(int *addrtype);
char **My_addrs(int *pfamily);

#endif

