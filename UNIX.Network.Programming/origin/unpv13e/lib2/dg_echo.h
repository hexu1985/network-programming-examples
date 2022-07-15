#ifndef __unp_dg_echo_h
#define __unp_dg_echo_h

#include "wrapsock.h"
#include "wrapstdio.h"

void dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen);

#endif
