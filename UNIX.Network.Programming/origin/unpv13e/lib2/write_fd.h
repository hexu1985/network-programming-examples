#ifndef __unp_write_fd_h
#define __unp_write_fd_h

#include <sys/socket.h>
#include "error.h"

/* define if struct msghdr contains the msg_control member */
#define HAVE_MSGHDR_MSG_CONTROL 1

ssize_t write_fd(int fd, void *ptr, size_t nbytes, int sendfd);
ssize_t Write_fd(int fd, void *ptr, size_t nbytes, int sendfd);

#endif

