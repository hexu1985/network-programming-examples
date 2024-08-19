/* include main */
#include	"unpipc.h"
#include	"mysemaphore.h"

#define	NBUFF	 	 10
#define	MAXNTHREADS	100
#define	SEM_MUTEX	"tmp/test/mutex"	 	/* these are args to px_ipc_name() */
#define	SEM_NEMPTY	"tmp/test/nempty"
#define	SEM_NSTORED	"tmp/test/nstored"

int		nitems, nproducers;		/* read-only by producer and consumer */

struct {	/* data shared by producers and consumer */
  int	buff[NBUFF];
  int	nput;
  int	nputval;
  mysem_t	*mutex, *nempty, *nstored;
} shared;

void	*produce(void *), *consume(void *);

int
main(int argc, char **argv)
{
	int		i, count[MAXNTHREADS];
	pthread_t	tid_produce[MAXNTHREADS], tid_consume;

	if (argc != 3)
		err_quit("usage: prodcons3 <#items> <#producers>");
	nitems = atoi(argv[1]);
	nproducers = min(atoi(argv[2]), MAXNTHREADS);

		/* 4initialize three semaphores */
	shared.mutex = Mysem_open(Px_ipc_name(SEM_MUTEX), O_CREAT | O_EXCL,
							FILE_MODE, 1);
	shared.nempty = Mysem_open(Px_ipc_name(SEM_NEMPTY), O_CREAT | O_EXCL,
							 FILE_MODE, NBUFF);
	shared.nstored = Mysem_open(Px_ipc_name(SEM_NSTORED), O_CREAT | O_EXCL,
							  FILE_MODE, 0);

		/* 4create all producers and one consumer */
	Set_concurrency(nproducers + 1);
	for (i = 0; i < nproducers; i++) {
		count[i] = 0;
		Pthread_create(&tid_produce[i], NULL, produce, &count[i]);
	}
	Pthread_create(&tid_consume, NULL, consume, NULL);

		/* 4wait for all producers and the consumer */
	for (i = 0; i < nproducers; i++) {
		Pthread_join(tid_produce[i], NULL);
		printf("count[%d] = %d\n", i, count[i]);	
	}
	Pthread_join(tid_consume, NULL);

	Mysem_unlink(Px_ipc_name(SEM_MUTEX));
	Mysem_unlink(Px_ipc_name(SEM_NEMPTY));
	Mysem_unlink(Px_ipc_name(SEM_NSTORED));
	exit(0);
}
/* end main */

/* include produce */
void *
produce(void *arg)
{
	for ( ; ; ) {
		Mysem_wait(shared.nempty);	/* wait for at least 1 empty slot */
		Mysem_wait(shared.mutex);

		if (shared.nput >= nitems) {
			Mysem_post(shared.nempty);
			Mysem_post(shared.mutex);
			return(NULL);			/* all done */
		}

		shared.buff[shared.nput % NBUFF] = shared.nputval;
		shared.nput++;
		shared.nputval++;

		Mysem_post(shared.mutex);
		Mysem_post(shared.nstored);	/* 1 more stored item */
		*((int *) arg) += 1;
	}
}
/* end produce */

/* include consume */
void *
consume(void *arg)
{
	int		i;

	for (i = 0; i < nitems; i++) {
		Mysem_wait(shared.nstored);		/* wait for at least 1 stored item */
		Mysem_wait(shared.mutex);

		if (shared.buff[i % NBUFF] != i)
			printf("error: buff[%d] = %d\n", i, shared.buff[i % NBUFF]);

		Mysem_post(shared.mutex);
		Mysem_post(shared.nempty);		/* 1 more empty slot */
	}
	return(NULL);
}
/* end consume */
