/* include sem_close */
#include	"unpipc.h"
#include	"mysemaphore.h"

int
mysem_close(mysem_t *sem)
{
	if (sem->sem_magic != SEM_MAGIC) {
		errno = EINVAL;
		return(-1);
	}
	sem->sem_magic = 0;		/* just in case */

	free(sem);
	return(0);
}
/* end sem_close */

void
Mysem_close(mysem_t *sem)
{
	if (mysem_close(sem) == -1)
		err_sys("mysem_close error");
}

