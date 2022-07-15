#include	"unpipc.h"
#include	"pthread_rwlock.h"

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

void *thread1(void *arg);

void *thread2(void *arg);

int main()
{
	pthread_t tid1, tid2;

	Pthread_rwlock_wrlock(&rwlock);	/* parent read locks entire file */
	printf("%s: parent has write lock\n", gf_time());

	Pthread_create(&tid1, NULL, thread1, NULL);
	Pthread_create(&tid2, NULL, thread2, NULL);

	/* 4parent */
	sleep(5);
	Pthread_rwlock_unlock(&rwlock);
	printf("%s: parent releases write lock\n", gf_time());

	Pthread_join(tid1, NULL);
	Pthread_join(tid2, NULL);

	exit(0);
}

void *thread1(void *arg)
{
	sleep(3);
	printf("%s: first child tries to obtain write lock\n", gf_time());
	Pthread_rwlock_wrlock(&rwlock);	/* this should block */
	printf("%s: first child obtains write lock\n", gf_time());

	sleep(2);
	Pthread_rwlock_unlock(&rwlock);
	printf("%s: first child releases write lock\n", gf_time());
	return NULL;
}

void *thread2(void *arg)
{
		/* 4second child */
	sleep(1);
	printf("%s: second child tries to obtain read lock\n", gf_time());
	Pthread_rwlock_rdlock(&rwlock);
	printf("%s: second child obtains read lock\n", gf_time());

	sleep(4);
	Pthread_rwlock_unlock(&rwlock);
	printf("%s: second child releases read lock\n", gf_time());
	return NULL;
}
