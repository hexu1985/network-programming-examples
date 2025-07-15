#ifndef __unp_wrapunix_h
#define __unp_wrapunix_h

#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdarg.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

#ifdef __linux__
#include <linux/sysctl.h>
#else
#include <sys/sysctl.h>
#endif

#ifdef	HAVE_MQUEUE_H
# include	<mqueue.h>		/* Posix message queues */
#endif

#ifdef	HAVE_SEMAPHORE_H
# include	<semaphore.h>	/* Posix semaphores */

#ifndef	SEM_FAILED
#define	SEM_FAILED	((sem_t *)(-1))
#endif

#endif

#ifdef	HAVE_SYS_MMAN_H
# include	<sys/mman.h>	/* Posix shared memory */
#endif

#ifndef	MAP_FAILED
#define	MAP_FAILED	((void *)(-1))
#endif

#ifdef	HAVE_SYS_IPC_H
# include	<sys/ipc.h>		/* System V IPC */
#endif

#ifdef	HAVE_SYS_MSG_H
# include	<sys/msg.h>		/* System V message queues */
#endif

#ifdef	HAVE_SYS_SEM_H
#ifdef	__bsdi__
#undef	HAVE_SYS_SEM_H		/* hack: BSDI's semctl() prototype is wrong */
#else
# include	<sys/sem.h>		/* System V semaphores */
#endif

#ifndef	HAVE_SEMUN_UNION
/* $$.It semun$$ */
union semun {				/* define union for semctl() */
  int              val;
  struct semid_ds *buf;
  unsigned short  *array;
};
#endif
#endif	/* HAVE_SYS_SEM_H */

#ifdef	HAVE_SYS_SHM_H
# include	<sys/shm.h>		/* System V shared memory */
#endif

#include "error.h"

#define HAVE_MKSTEMP 1

/*
 * In our wrappers for open(), mq_open(), and sem_open() we handle the
 * optional arguments using the va_XXX() macros.  But one of the optional
 * arguments is of type "mode_t" and this breaks under BSD/OS because it
 * uses a 16-bit integer for this datatype.  But when our wrapper function
 * is called, the compiler expands the 16-bit short integer to a 32-bit
 * integer.  This breaks our call to va_arg().  All we can do is the
 * following hack.  Other systems in addition to BSD/OS might have this
 * problem too ...
 */

#ifdef  __bsdi__
#define va_mode_t   int
#else
#define va_mode_t   mode_t
#endif
/* $$.ix [va_mode_t]~datatype,~definition~of$$ */

/* prototypes for our Unix wrapper functions: see {Sec errors} */
void    *Calloc(size_t, size_t);
void     Close(int);
void     Dup2(int, int);

#ifdef HAVE_FATTACH
void     Fattach(int, const char *);
#endif

int      Fcntl(int, int, ...);
pid_t    Fork(void);
long     Fpathconf(int, int);
void     Fstat(int, struct stat *);

#ifdef HAVE_SYS_IPC_H
key_t    Ftok(const char *, int);
#endif

void     Ftruncate(int, off_t);
int      Getopt(int, char *const *, const char *);

void     Gettimeofday(struct timeval *, void *);
int      Ioctl(int, int, void *);
void     Kill(pid_t, int);
off_t    Lseek(int, off_t, int);
void    *Malloc(size_t);
void     Mkfifo(const char *, mode_t);
void     Mktemp(char *);
int      Mkstemp(char *);

//#ifdef HAVE_SYS_MMAN_H
void    *Mmap(void *, size_t, int, int, int, off_t);
void     Munmap(void *, size_t);
//#endif

#ifdef HAVE_MQUEUE_H
mqd_t    Mq_open(const char *, int, ...);
void     Mq_close(mqd_t);
void     Mq_unlink(const char *);
void     Mq_send(mqd_t, const char *, size_t, unsigned int);
ssize_t  Mq_receive(mqd_t, char *, size_t, unsigned int *);
void     Mq_notify(mqd_t, const struct sigevent *);
void     Mq_getattr(mqd_t, struct mq_attr *);
void     Mq_setattr(mqd_t, const struct mq_attr *, struct mq_attr *);
#endif

#ifdef	HAVE_SEMAPHORE_H
sem_t   *Sem_open(const char *, int, ...);
void     Sem_close(sem_t *);
void     Sem_unlink(const char *);
void     Sem_init(sem_t *, int, unsigned int);
void     Sem_destroy(sem_t *);
void     Sem_wait(sem_t *);
int      Sem_trywait(sem_t *);
void     Sem_post(sem_t *);
void     Sem_getvalue(sem_t *, int *);
#endif

#ifdef	HAVE_SHM_OPEN_PROTO
			/* 4Posix shared memory */
int		 Shm_open(const char *, int, mode_t);
void	 Shm_unlink(const char *);
#endif

#ifdef	HAVE_SYS_MSG_H
			/* 4System V message queues */
int		 Msgget(key_t key, int flag);
void	 Msgctl(int, int, struct msqid_ds *);
void	 Msgsnd(int, const void *, size_t, int);
ssize_t	 Msgrcv(int, void *, size_t, int, int);
#endif	/* HAVE_SYS_MSG_H */

#ifdef	HAVE_SYS_SEM_H
			/* 4System V semaphores */
int		 Semget(key_t, int, int);
int		 Semctl(int, int, int, ...);
void	 Semop(int, struct sembuf *, size_t);
#endif	/* HAVE_SYS_SEM_H */

#ifdef	HAVE_SYS_SHM_H
			/* 4System V shared memory */
int		 Shmget(key_t, size_t, int);
void	*Shmat(int, const void *, int);
void	 Shmdt(const void *);
void	 Shmctl(int, int, struct shmid_ds *);
#endif	/* HAVE_SYS_SHM_H */

int      Open(const char *, int , ...);
void     Pipe(int *fds);
ssize_t  Read(int, void *, size_t);
void     Sigaddset(sigset_t *, int);
void     Sigdelset(sigset_t *, int);
void     Sigemptyset(sigset_t *);
void     Sigfillset(sigset_t *);
int      Sigismember(const sigset_t *, int);
void     Sigpending(sigset_t *);
void     Sigprocmask(int, const sigset_t *, sigset_t *);

#ifdef	HAVE_SIGINFO_T_STRUCT
void	 Sigqueue(pid_t, int, const union sigval);
#endif

char    *Strdup(const char *);
long     Sysconf(int);
void     Sysctl(int *, u_int, void *, size_t *, void *, size_t);
void     Unlink(const char *);
void	*Valloc(size_t);
pid_t    Wait(int *);
pid_t    Waitpid(pid_t, int *, int);
void     Write(int, void *, size_t);
long     Pathconf(const char *pathname, int name);
int		 Select(int, fd_set *, fd_set *, fd_set *, struct timeval *);
void     Sigwait(const sigset_t *set, int *signo);
void	 Stat(const char *, struct stat *);

/* Define to 1 if you have the `mkstemp' function. */
#define HAVE_MKSTEMP 1

/* Define to 1 if you have the <sys/sysctl.h> header file. */
#define HAVE_SYS_SYSCTL_H 1

#endif
