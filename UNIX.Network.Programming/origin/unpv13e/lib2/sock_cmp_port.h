#ifndef	__unp_sock_cmp_port_h
#define	__unp_sock_cmp_port_h

#include <netinet/in.h>

/* Define to 1 if the system supports IPv6 */
#define IPV6    1

int sock_cmp_port(const struct sockaddr *sa1, const struct sockaddr *sa2,
			 socklen_t salen);

#endif

