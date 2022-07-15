#ifndef	__unp_wraplib_h
#define	__unp_wraplib_h

#include "error.h"
#include <arpa/inet.h>

const char		*Inet_ntop(int, const void *, char *, size_t);
void			 Inet_pton(int, const char *, void *);

#endif
