#ifndef	__unp_sock_set_addr_h
#define	__unp_sock_set_addr_h

#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

/* Define to 1 if the system supports IPv6 */
#define IPV6    1

void sock_set_addr(struct sockaddr *, socklen_t, const void *);

#endif

