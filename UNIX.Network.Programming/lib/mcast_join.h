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

int      mcast_join(int, const SA *, socklen_t, const char *, u_int);
int      mcast_join_source_group(int, const SA *, socklen_t, const SA *, socklen_t, const char *, u_int);
int      mcast_block_source(int, const SA *, socklen_t, const SA *, socklen_t);
int      mcast_unblock_source(int, const SA *, socklen_t, const SA *, socklen_t);
void     Mcast_join(int, const SA *, socklen_t, const char *, u_int);
void     Mcast_join_source_group(int, const SA *, socklen_t, const SA *, socklen_t, const char *, u_int);
void     Mcast_block_source(int, const SA *, socklen_t, const SA *, socklen_t);
void     Mcast_unblock_source(int, const SA *, socklen_t, const SA *, socklen_t);

#endif
