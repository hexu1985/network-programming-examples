#ifndef __unp_tcp_connect_h
#define __unp_tcp_connect_h

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "error.h"
#include "wrapunix.h"

int tcp_connect(const char *host, const char *serv);
int Tcp_connect(const char *host, const char *serv);

#endif
