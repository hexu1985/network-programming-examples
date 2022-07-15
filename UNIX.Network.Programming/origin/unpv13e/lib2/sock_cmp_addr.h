#ifndef	__unp_sock_cmp_addr_h
#define	__unp_sock_cmp_addr_h

#include <netinet/in.h>
#include <string.h>
#include <sys/un.h>

/* Define to 1 if the system supports IPv6 */
#define IPV6    1

int sock_cmp_addr(const struct sockaddr *sa1, const struct sockaddr *sa2,
			 socklen_t salen);

#endif
