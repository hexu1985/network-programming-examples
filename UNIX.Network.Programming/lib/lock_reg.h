#ifndef __unp_lock_reg_h
#define __unp_lock_reg_h

#include <fcntl.h>
#include "error.h"

        /* our record locking macros */
#define read_lock(fd, offset, whence, len) \
                lock_reg(fd, F_SETLK, F_RDLCK, offset, whence, len)
#define readw_lock(fd, offset, whence, len) \
                lock_reg(fd, F_SETLKW, F_RDLCK, offset, whence, len)
#define write_lock(fd, offset, whence, len) \
                lock_reg(fd, F_SETLK, F_WRLCK, offset, whence, len)
#define writew_lock(fd, offset, whence, len) \
                lock_reg(fd, F_SETLKW, F_WRLCK, offset, whence, len)
#define un_lock(fd, offset, whence, len) \
                lock_reg(fd, F_SETLK, F_UNLCK, offset, whence, len)
#define is_read_lockable(fd, offset, whence, len) \
                lock_test(fd, F_RDLCK, offset, whence, len)
#define is_write_lockable(fd, offset, whence, len) \
                lock_test(fd, F_WRLCK, offset, whence, len)
/* end unpipch */

#define Read_lock(fd, offset, whence, len) \
                Lock_reg(fd, F_SETLK, F_RDLCK, offset, whence, len)
#define Readw_lock(fd, offset, whence, len) \
                Lock_reg(fd, F_SETLKW, F_RDLCK, offset, whence, len)
#define Write_lock(fd, offset, whence, len) \
                Lock_reg(fd, F_SETLK, F_WRLCK, offset, whence, len)
#define Writew_lock(fd, offset, whence, len) \
                Lock_reg(fd, F_SETLKW, F_WRLCK, offset, whence, len)
#define Un_lock(fd, offset, whence, len) \
                Lock_reg(fd, F_SETLK, F_UNLCK, offset, whence, len)
#define Is_read_lockable(fd, offset, whence, len) \
                Lock_test(fd, F_RDLCK, offset, whence, len)
#define Is_write_lockable(fd, offset, whence, len) \
                Lock_test(fd, F_WRLCK, offset, whence, len)

int     lock_reg(int, int, int, off_t, int, off_t);
void    Lock_reg(int, int, int, off_t, int, off_t);

#endif

