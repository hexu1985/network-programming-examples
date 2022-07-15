#ifndef __unp_writable_timeo_h
#define __unp_writable_timeo_h

#include <sys/select.h>
#include "error.h"

int writable_timeo(int fd, int sec);
int Writable_timeo(int fd, int sec);

#endif

