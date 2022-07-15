#ifndef __unp_mcast_leave_h
#define __unp_mcast_leave_h

#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include "error.h"
#include "family_to_level.h"

#define	SA	struct sockaddr

/* Define to 1 if the system supports IPv6 */
#define IPV6    1

int mcast_leave(int sockfd, const SA *grp, socklen_t grplen);
int mcast_leave_source_group(int sockfd, const SA *src, socklen_t srclen, const SA *grp, socklen_t grplen);
void Mcast_leave(int sockfd, const SA *grp, socklen_t grplen);
void Mcast_leave_source_group(int sockfd, const SA *src, socklen_t srclen, const SA *grp, socklen_t grplen);

#endif
