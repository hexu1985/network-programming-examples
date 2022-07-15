#ifndef	__unp_sock_set_port_h
#define	__unp_sock_set_port_h

#include <netinet/in.h>
#include <arpa/inet.h>

/* Define to 1 if the system supports IPv6 */
#define IPV6    1

void sock_set_port(struct sockaddr *, socklen_t, int);

#endif

