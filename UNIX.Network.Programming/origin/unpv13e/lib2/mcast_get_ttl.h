#ifndef __unp_mcast_get_ttl_h
#define __unp_mcast_get_ttl_h

#include <arpa/inet.h>
#include <sys/socket.h>
#include "error.h"
#include "sockfd_to_family.h"

/* Define to 1 if the system supports IPv6 */
#define IPV6    1

int mcast_get_ttl(int sockfd);
int Mcast_get_ttl(int sockfd);

#endif
