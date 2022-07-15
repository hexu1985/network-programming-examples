#ifndef __unp_host_serv_h
#define __unp_host_serv_h

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "error.h"

struct addrinfo     *host_serv(const char *, const char *, int, int);
struct addrinfo     *Host_serv(const char *, const char *, int, int);

#endif
