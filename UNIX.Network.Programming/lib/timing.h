#ifndef __unp_timing_h
#define __unp_timing_h

#include <unistd.h>
#include "tv_sub.h"
#include "error.h"

int      start_time(void);
double   stop_time(void);
int      touch(void *, int);
void     Start_time(void);
double   Stop_time(void);
void     Touch(void *, int);

#endif
