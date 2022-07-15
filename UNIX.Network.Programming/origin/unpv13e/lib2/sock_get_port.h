#ifndef	__unp_sock_get_port_h
#define	__unp_sock_get_port_h

#include <netinet/in.h>
#include <arpa/inet.h>

/* Define to 1 if the system supports IPv6 */
#define IPV6    1

int sock_get_port(const struct sockaddr *sa, socklen_t salen);

#endif

