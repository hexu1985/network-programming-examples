#ifndef __unp_readable_timeo_h
#define __unp_readable_timeo_h

#include <sys/select.h>
#include "error.h"

int readable_timeo(int fd, int sec);
int Readable_timeo(int fd, int sec);

#endif

