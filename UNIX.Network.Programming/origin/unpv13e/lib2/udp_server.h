#ifndef __unp_udp_server_h
#define __unp_udp_server_h

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "error.h"
#include "wrapunix.h"

int udp_server(const char *host, const char *serv, socklen_t *addrlenp);
int Udp_server(const char *host, const char *serv, socklen_t *addrlenp);

#endif
