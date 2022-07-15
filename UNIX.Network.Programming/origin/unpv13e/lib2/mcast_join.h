#ifndef __unp_mcast_join_h
#define __unp_mcast_join_h

#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include "error.h"
#include "family_to_level.h"

#define	SA	struct sockaddr

/* Define to 1 if the system supports IPv6 */
#define IPV6    1

int mcast_join(int sockfd, const SA *grp, socklen_t grplen, const char *ifname, u_int ifindex);
int mcast_join_source_group(int sockfd, const SA *src, socklen_t srclen, const SA *grp, socklen_t grplen, const char *ifname, u_int ifindex);
int mcast_block_source(int sockfd, const SA *src, socklen_t srclen, const SA *grp, socklen_t grplen);
int mcast_unblock_source(int sockfd, const SA *src, socklen_t srclen, const SA *grp, socklen_t grplen);
void Mcast_join(int sockfd, const SA *grp, socklen_t grplen, const char *ifname, u_int ifindex);
void Mcast_join_source_group(int sockfd, const SA *src, socklen_t srclen, const SA *grp, socklen_t grplen, const char *ifname, u_int ifindex);
void Mcast_block_source(int sockfd, const SA *src, socklen_t srclen, const SA *grp, socklen_t grplen);
void Mcast_unblock_source(int sockfd, const SA *src, socklen_t srclen, const SA *grp, socklen_t grplen);

#endif
