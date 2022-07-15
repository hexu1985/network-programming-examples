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

int     mcast_leave(int, const SA *, socklen_t);
int     mcast_leave_source_group(int, const SA *, socklen_t, const SA *, socklen_t);
void    Mcast_leave(int, const SA *, socklen_t);
void    Mcast_leave_source_group(int, const SA *, socklen_t, const SA *, socklen_t);

#endif
