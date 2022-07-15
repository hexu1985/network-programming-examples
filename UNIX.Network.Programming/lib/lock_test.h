#ifndef __unp_lock_test_h
#define __unp_lock_test_h

#include <fcntl.h>
#include "error.h"

pid_t   lock_test(int, int, off_t, int, off_t);
pid_t   Lock_test(int, int, off_t, int, off_t);

#endif

