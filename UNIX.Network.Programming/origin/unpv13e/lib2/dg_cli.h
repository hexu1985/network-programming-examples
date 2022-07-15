#ifndef __unp_dg_cli_h
#define __unp_dg_cli_h

#include "wrapsock.h"
#include "wrapstdio.h"

void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen);

#endif
