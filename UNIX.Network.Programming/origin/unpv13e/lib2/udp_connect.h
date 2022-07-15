#ifndef __unp_udp_connect_h
#define __unp_udp_connect_h

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "error.h"
#include "wrapunix.h"

int udp_connect(const char *host, const char *serv);
int Udp_connect(const char *host, const char *serv);

#endif
