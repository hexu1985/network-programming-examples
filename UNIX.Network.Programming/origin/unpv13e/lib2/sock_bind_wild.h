#ifndef __unp_sock_bind_wild_h
#define __unp_sock_bind_wild_h

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "error.h"

#define	SA	struct sockaddr

/* Define to 1 if the system supports IPv6 */
#define IPV6    1

int sock_bind_wild(int sockfd, int family);
int Sock_bind_wild(int sockfd, int family);

#endif

