#ifndef	__unp_wrappthread_h
#define	__unp_wrappthread_h

#include "error.h"
#include <pthread.h>
#include <signal.h>
#include <errno.h>

void    Pthread_attr_init(pthread_attr_t *);
void    Pthread_attr_destroy(pthread_attr_t *);
void    Pthread_attr_setdetachstate(pthread_attr_t *, int);
void    Pthread_attr_setscope(pthread_attr_t *, int);

void	Pthread_create(pthread_t *, const pthread_attr_t *, void * (*)(void *), void *);
void	Pthread_join(pthread_t, void **);
void	Pthread_detach(pthread_t);
void	Pthread_kill(pthread_t, int);

void	Pthread_mutexattr_init(pthread_mutexattr_t *);
void    Pthread_mutexattr_destroy(pthread_mutexattr_t *);
#ifdef	_POSIX_THREAD_PROCESS_SHARED
void	Pthread_mutexattr_setpshared(pthread_mutexattr_t *, int);
#endif

void    Pthread_condattr_init(pthread_condattr_t *);
void    Pthread_condattr_destroy(pthread_condattr_t *);
#ifdef	_POSIX_THREAD_PROCESS_SHARED
void    Pthread_condattr_setpshared(pthread_condattr_t *, int);
#endif

void	Pthread_mutex_init(pthread_mutex_t *, pthread_mutexattr_t *);
void    Pthread_mutex_destroy(pthread_mutex_t *);
void	Pthread_mutex_lock(pthread_mutex_t *);
void	Pthread_mutex_unlock(pthread_mutex_t *);

void	Pthread_cond_broadcast(pthread_cond_t *);
void	Pthread_cond_signal(pthread_cond_t *);
void	Pthread_cond_wait(pthread_cond_t *, pthread_mutex_t *);
void	Pthread_cond_timedwait(pthread_cond_t *, pthread_mutex_t *, const struct timespec *);

void	Pthread_once(pthread_once_t *, void (*)(void));
void	Pthread_key_create(pthread_key_t *, void (*)(void *));
void    Pthread_setcancelstate(int, int *);
void	Pthread_setspecific(pthread_key_t, const void *);

long    pr_thread_id(pthread_t *);

void	Pthread_rwlock_destroy(pthread_rwlock_t *);
void	Pthread_rwlock_init(pthread_rwlock_t *, pthread_rwlockattr_t *);
void	Pthread_rwlock_rdlock(pthread_rwlock_t *);
int		Pthread_rwlock_tryrdlock(pthread_rwlock_t *);
int		Pthread_rwlock_trywrlock(pthread_rwlock_t *);
void	Pthread_rwlock_unlock(pthread_rwlock_t *);
void	Pthread_rwlock_wrlock(pthread_rwlock_t *);

#endif
