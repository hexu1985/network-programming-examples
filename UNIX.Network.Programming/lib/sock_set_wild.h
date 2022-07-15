#ifndef	__unp_sock_set_wild_h
#define	__unp_sock_set_wild_h

#include <netinet/in.h>
#include <arpa/inet.h>
#include "sock_set_addr.h"

/* Define to 1 if the system supports IPv6 */
#define IPV6    1

void sock_set_wild(struct sockaddr *, socklen_t);

#endif

