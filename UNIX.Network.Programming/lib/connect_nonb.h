#ifndef __unp_connect_nonb_h
#define __unp_connect_nonb_h

#include <sys/types.h>
#include <sys/socket.h>

#include "error.h"
#include "wrapunix.h"
#include "wrapsock.h"

#define	SA	struct sockaddr

int		 connect_nonb(int, const SA *, socklen_t, int);

#endif
