#ifndef __unp_sockfd_to_family_h
#define __unp_sockfd_to_family_h

#include <sys/socket.h>
#include "error.h"

#define	SA	struct sockaddr

int sockfd_to_family(int);
int Sockfd_to_family(int);

#endif
