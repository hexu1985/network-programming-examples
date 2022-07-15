CC = gcc
CFLAGS = -I. -g -O2 -Wall -DHAVE_SIGWAIT -DHAVE_MQUEUE_H -DHAVE_SEMAPHORE_H -DHAVE_SYS_MMAN_H -DHAVE_SYS_MSG_H -D_REENTRANT -DHAVE_EPOLL -DHAVE_SYS_IPC_H -DHAVE_SYS_SEM_H -DHAVE_SYS_SHM_H -DPOSIX_IPC_PREFIX='"/"' -DHAVE_SHM_OPEN_PROTO -DHAVE_SIGINFO_T_STRUCT -DNOTDEF 
RANLIB = ranlib

# Following is the main library, built from all the object files
# in the lib/ and libfree/ directories.
LIBUNP_NAME = libunp.a
LIBUNPIPC_NAME = libunpipc.a
LIBUNPALL_NAME = libunpall.a

# Following are all the object files to create in the lib/ directory.
LIBUNP_OBJS = connect_nonb.o connect_timeo.o daemon_inetd.o daemon_init.o dg_cli.o dg_echo.o error.o get_ifi_info.o gf_time.o host_serv.o family_to_level.o mcast_leave.o mcast_join.o mcast_get_if.o mcast_get_loop.o mcast_get_ttl.o mcast_set_if.o mcast_set_loop.o mcast_set_ttl.o my_addrs.o read_fd.o readline.o readn.o readable_timeo.o rtt.o wrapsignal.o signal_intr.o sock_bind_wild.o sock_cmp_addr.o sock_cmp_port.o sock_ntop.o sock_ntop_host.o sock_get_port.o sock_set_addr.o sock_set_port.o sock_set_wild.o sockfd_to_family.o str_cli.o str_echo.o tcp_connect.o tcp_listen.o tv_sub.o udp_client.o udp_connect.o udp_server.o wraplib.o wrapsock.o wrapstdio.o wrappthread.o wrapunix.o write_fd.o writen.o writable_timeo.o
LIBUNPIPC_OBJS =  daemon_inetd.o daemon_init.o error.o gf_time.o lock_reg.o lock_test.o my_shm.o px_ipc_name.o readable_timeo.o readline.o readn.o set_concurrency.o set_nonblock.o wrapsignal.o signal_intr.o sleep_us.o signal_rt.o signal_rt_intr.o timing.o tv_sub.o wrappthread.o wrapstdio.o wrapunix.o writable_timeo.o writen.o
LIBUNPALL_SRCS = $(shell ls *.c)
LIBUNPALL_OBJS = $(LIBUNPALL_SRCS:%.c=%.o)

PROGS = ${LIBUNP_NAME} ${LIBUNPIPC_NAME} ${LIBUNPALL_NAME}

CLEANFILES = core core.* *.core *.o temp.* *.out typescript* \
		*.lc *.lh *.bsdi *.sparc *.uw *.a

all:	${LIBUNP_OBJS} ${LIBUNPIPC_OBJS} ${LIBUNPALL_OBJS}
		ar rv ${LIBUNP_NAME} ${LIBUNP_OBJS}
		${RANLIB} ${LIBUNP_NAME}
		ar rv ${LIBUNPALL_NAME} ${LIBUNPALL_OBJS}
		${RANLIB} ${LIBUNPALL_NAME}
		ar rv ${LIBUNPIPC_NAME} ${LIBUNPIPC_OBJS}
		${RANLIB} ${LIBUNPIPC_NAME}

clean:
		rm -f ${PROGS} ${CLEANFILES}

distclean: clean
