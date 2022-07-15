#ifndef __unp_signal_rt_h
#define __unp_signal_rt_h

#include <signal.h>
#include "error.h"

typedef void    Sigfunc_rt(int, siginfo_t *, void *);

Sigfunc_rt *signal_rt(int, Sigfunc_rt *);
Sigfunc_rt *Signal_rt(int, Sigfunc_rt *);

#endif
