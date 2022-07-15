#ifndef __unp_sleep_us_h
#define __unp_sleep_us_h

#include <sys/select.h>
#include "error.h"

int     sleep_us(unsigned int);
void    Sleep_us(unsigned int);

#endif
