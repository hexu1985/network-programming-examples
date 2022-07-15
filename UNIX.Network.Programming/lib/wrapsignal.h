#ifndef __unp_signal_h
#define __unp_signal_h

#include <signal.h>
#include "error.h"

typedef	void	Sigfunc(int);	/* for signal handlers */

Sigfunc *signal(int, Sigfunc *);
Sigfunc *Signal(int, Sigfunc *);

#endif
