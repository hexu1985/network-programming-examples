/* include rwlockh */
#ifndef	__pthread_rwlock_h
#define	__pthread_rwlock_h

/** 为了解决和pthread_rwlock的冲突 */
#define pthread_rwlock_t            mypthread_rwlock_t
#undef  PTHREAD_RWLOCK_INITIALIZER
#define pthread_rwlock_destroy      mypthread_rwlock_destroy
#define pthread_rwlock_init         mypthread_rwlock_init
#define pthread_rwlock_rdlock       mypthread_rwlock_rdlock
#define pthread_rwlock_tryrdlock    mypthread_rwlock_tryrdlock
#define pthread_rwlock_trywrlock    mypthread_rwlock_trywrlock
#define pthread_rwlock_unlock       mypthread_rwlock_unlock
#define pthread_rwlock_wrlock       mypthread_rwlock_wrlock
#define pthread_rwlockattr_t        mypthread_rwlockattr_t
#define Pthread_rwlock_destroy      Mypthread_rwlock_destroy
#define Pthread_rwlock_init         Mypthread_rwlock_init
#define Pthread_rwlock_rdlock       Mypthread_rwlock_rdlock
#define Pthread_rwlock_tryrdlock    Mypthread_rwlock_tryrdlock
#define Pthread_rwlock_trywrlock    Mypthread_rwlock_trywrlock
#define Pthread_rwlock_unlock       Mypthread_rwlock_unlock
#define Pthread_rwlock_wrlock       Mypthread_rwlock_wrlock
#define Pthread_rwlockattr_t        Mypthread_rwlockattr_t


typedef struct {
  pthread_mutex_t	rw_mutex;		/* basic lock on this struct */
  pthread_cond_t	rw_condreaders;	/* for reader threads waiting */
  pthread_cond_t	rw_condwriters;	/* for writer threads waiting */
  int				rw_magic;		/* for error checking */
  int				rw_nwaitreaders;/* the number waiting */
  int				rw_nwaitwriters;/* the number waiting */
  int				rw_refcount;
	/* 4-1 if writer has the lock, else # readers holding the lock */
} pthread_rwlock_t;

#define	RW_MAGIC	0x19283746

	/* 4following must have same order as elements in struct above */
#define	PTHREAD_RWLOCK_INITIALIZER	{ PTHREAD_MUTEX_INITIALIZER, \
			PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER, \
			RW_MAGIC, 0, 0, 0 }

typedef	int		pthread_rwlockattr_t;	/* dummy; not supported */

	/* 4function prototypes */
int		pthread_rwlock_destroy(pthread_rwlock_t *);
int		pthread_rwlock_init(pthread_rwlock_t *, pthread_rwlockattr_t *);
int		pthread_rwlock_rdlock(pthread_rwlock_t *);
int		pthread_rwlock_tryrdlock(pthread_rwlock_t *);
int		pthread_rwlock_trywrlock(pthread_rwlock_t *);
int		pthread_rwlock_unlock(pthread_rwlock_t *);
int		pthread_rwlock_wrlock(pthread_rwlock_t *);
/* $$.bp$$ */
	/* 4and our wrapper functions */
void	Pthread_rwlock_destroy(pthread_rwlock_t *);
void	Pthread_rwlock_init(pthread_rwlock_t *, pthread_rwlockattr_t *);
void	Pthread_rwlock_rdlock(pthread_rwlock_t *);
int		Pthread_rwlock_tryrdlock(pthread_rwlock_t *);
int		Pthread_rwlock_trywrlock(pthread_rwlock_t *);
void	Pthread_rwlock_unlock(pthread_rwlock_t *);
void	Pthread_rwlock_wrlock(pthread_rwlock_t *);

#endif	// __pthread_rwlock_h
/* end rwlockh */
