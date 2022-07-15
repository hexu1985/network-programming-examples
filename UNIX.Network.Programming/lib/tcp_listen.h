#ifndef __unp_tcp_listen_h
#define __unp_tcp_listen_h

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "error.h"
#include "wrapunix.h"
#include "wrapsock.h"

/* Following could be derived from SOMAXCONN in <sys/socket.h>, but many
   kernels still #define it as 5, while actually supporting many more */
#define	LISTENQ		1024	/* 2nd argument to listen() */

int tcp_listen(const char *, const char *, socklen_t *);
int Tcp_listen(const char *, const char *, socklen_t *);

#endif
