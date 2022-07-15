#ifndef __unp_connect_timeo_h
#define __unp_connect_timeo_h

#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include "error.h"
#include "wrapsignal.h"

#define	SA	struct sockaddr

int      connect_timeo(int, const SA *, socklen_t, int);
void	 Connect_timeo(int, const SA *, socklen_t, int);

#endif
