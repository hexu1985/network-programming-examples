#ifndef __unp_read_fd_h
#define __unp_read_fd_h

#include <sys/socket.h>
#include "error.h"

/* define if struct msghdr contains the msg_control member */
#define HAVE_MSGHDR_MSG_CONTROL 1

ssize_t read_fd(int fd, void *ptr, size_t nbytes, int *recvfd);
ssize_t Read_fd(int fd, void *ptr, size_t nbytes, int *recvfd);

#endif

