/* include main */
#include	"unpipc.h"

void	 Rw_wrlock(pthread_rwlock_t *rwptr);
void	 Rw_unlock(pthread_rwlock_t *rwptr);

#define	MAXNTHREADS	100

int		nloop;

struct {
  pthread_rwlock_t	rwlock;			/* the posix datatype */
  long	counter;
} shared = { PTHREAD_RWLOCK_INITIALIZER };

void	*incr(void *);

int
main(int argc, char **argv)
{
	int		i, nthreads;
	pthread_t	tid[MAXNTHREADS];

	if (argc != 3)
		err_quit("usage: incr_rwlock1 <#loops> <#threads>");
	nloop = atoi(argv[1]);
	nthreads = min(atoi(argv[2]), MAXNTHREADS);

		/* 4obtain write lock */
	Rw_wrlock(&shared.rwlock);

		/* 4create all the threads */
	Set_concurrency(nthreads);
	for (i = 0; i < nthreads; i++) {
		Pthread_create(&tid[i], NULL, incr, NULL);
	}
		/* 4start the timer and release the write lock */
	Start_time();
	Rw_unlock(&shared.rwlock);

		/* 4wait for all the threads */
	for (i = 0; i < nthreads; i++) {
		Pthread_join(tid[i], NULL);
	}
	printf("microseconds: %.0f usec\n", Stop_time());
	if (shared.counter != nloop * nthreads)
		printf("error: counter = %ld\n", shared.counter);

	exit(0);
}
/* end main */

/* include incr */
void *
incr(void *arg)
{
	int		i;

	for (i = 0; i < nloop; i++) {
		Rw_wrlock(&shared.rwlock);
		shared.counter++;
		Rw_unlock(&shared.rwlock);
	}
	return(NULL);
}
/* end incr */

void
Rw_wrlock(pthread_rwlock_t *rwptr)
{
	int		n;

	if ( (n = pthread_rwlock_wrlock(rwptr)) == 0)
		return;
	errno = n;
	err_sys("rw_wrlock error");
}

void
Rw_unlock(pthread_rwlock_t *rwptr)
{
	int		n;

	if ( (n = pthread_rwlock_unlock(rwptr)) == 0)
		return;
	errno = n;
	err_sys("rw_unlock error");
}
