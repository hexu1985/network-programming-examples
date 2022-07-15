#include	"set_concurrency.h"

int pthread_setconcurrency(int new_level);

int
set_concurrency(int level)
{
	return pthread_setconcurrency(level);
}

void
Set_concurrency(int level)
{
	if (set_concurrency(level) != 0)
		err_sys("set_concurrency error");
}
