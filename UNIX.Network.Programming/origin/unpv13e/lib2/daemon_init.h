#ifndef __unp_daemon_init_h
#define __unp_daemon_init_h

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "wrapunix.h"
#include "wrapsignal.h"

int daemon_init(const char *pname, int facility);

#endif
