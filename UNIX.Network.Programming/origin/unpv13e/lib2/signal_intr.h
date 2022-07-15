#ifndef __unp_signal_intr_h
#define __unp_signal_intr_h

#include <signal.h>
#include "error.h"

typedef	void	Sigfunc(int);	/* for signal handlers */

Sigfunc *signal_intr(int signo, Sigfunc *func);
Sigfunc *Signal_intr(int signo, Sigfunc *func);

#endif

