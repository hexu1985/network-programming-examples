#ifndef __unp_mcast_set_if_h
#define __unp_mcast_set_if_h

#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "error.h"
#include "sockfd_to_family.h"

/* Define to 1 if the system supports IPv6 */
#define IPV6    1

int     mcast_set_if(int, const char *, u_int);
void    Mcast_set_if(int, const char *, u_int);

#endif
