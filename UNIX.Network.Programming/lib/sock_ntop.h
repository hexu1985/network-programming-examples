#ifndef	__unp_sock_ntop_h
#define	__unp_sock_ntop_h

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <string.h>
#include <stdio.h>
#include "error.h"

/* Define to 1 if the system supports IPv6 */
#define IPV6    1

char *sock_ntop(const struct sockaddr *, socklen_t);
char *Sock_ntop(const struct sockaddr *, socklen_t);

#endif

