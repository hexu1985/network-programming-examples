#ifndef __unp_unp_util_h
#define __unp_unp_util_h

#include <limits.h>

#ifndef OPEN_MAX
#define OPEN_MAX    1024
#endif

/* POSIX requires that an #include of <poll.h> DefinE INFTIM, but many
   systems still DefinE it in <sys/stropts.h>.  We don't want to include
   all the STREAMS stuff if it's not needed, so we just DefinE INFTIM here.
   This is the standard value, but there's no guarantee it is -1. */
#ifndef INFTIM
#define INFTIM          (-1)    /* infinite poll timeout */
#endif

/* Following could be derived from SOMAXCONN in <sys/socket.h>, but many
   kernels still #define it as 5, while actually supporting many more */
//#define LISTENQ     1024    /* 2nd argument to listen() */

/* Miscellaneous constants */
#define MAXLINE     4096    /* max text line length */
#define BUFFSIZE    8192    /* buffer size for reads and writes */

/* Define some port number that can be used for our examples */
#define SERV_PORT        9877           /* TCP and UDP */
#define SERV_PORT_STR   "9877"          /* TCP and UDP */
#define UNIXSTR_PATH    "/tmp/unix.str" /* Unix domain stream */
#define UNIXDG_PATH     "/tmp/unix.dg"  /* Unix domain datagram */

#define min(a,b)    ((a) < (b) ? (a) : (b))
#define max(a,b)    ((a) > (b) ? (a) : (b))

/** define in recvfromflags.c */
ssize_t recvfrom_flags(int fd, void *ptr, size_t nbytes, int *flagsp,
			   SA *sa, socklen_t *salenptr, struct unp_in_pktinfo *pktp);
ssize_t Recvfrom_flags(int fd, void *ptr, size_t nbytes, int *flagsp,
			   SA *sa, socklen_t *salenptr, struct unp_in_pktinfo *pktp);

/** define in sourceroute.c */
u_char *inet_srcrt_init(int type);
int inet_srcrt_add(char *hostptr);
void inet_srcrt_print(u_char *ptr, int len);

/** define in udp_server_reuseaddr.c */
int udp_server_reuseaddr(const char *host, const char *serv, socklen_t *addrlenp);
int Udp_server_reuseaddr(const char *host, const char *serv, socklen_t *addrlenp);

/** define in heartbeatcli.c */
void heartbeat_cli(int servfd_arg, int nsec_arg, int maxnprobes_arg);

/** define in heartbeatserv.c */
void heartbeat_serv(int servfd_arg, int nsec_arg, int maxnalarms_arg);

/** define in lock_fcntl.c */
void my_lock_init(char *pathname);
void my_lock_wait();
void my_lock_release();

#endif

