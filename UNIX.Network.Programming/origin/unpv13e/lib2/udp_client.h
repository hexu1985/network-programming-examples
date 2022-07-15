#ifndef __unp_udp_client_h
#define __unp_udp_client_h

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "error.h"
#include "wrapunix.h"

#define	SA	struct sockaddr

int udp_client(const char *host, const char *serv, SA **saptr, socklen_t *lenp);
int Udp_client(const char *host, const char *serv, SA **saptr, socklen_t *lenptr);

#endif
