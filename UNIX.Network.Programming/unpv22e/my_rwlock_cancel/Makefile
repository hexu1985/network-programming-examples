RM = rm -f
CC = gcc
INCLUDE = -I../../lib/
CFLAGS = -Wall -g -O2 ${INCLUDE}
LIBS = ../../lib/libunpipc.a -lpthread -lrt

PROGS =	test1 testcancel incr_rwlock1 \
		test2 test3 test4

FUNCS = pthread_rwlock_destroy.o pthread_rwlock_init.o \
		pthread_rwlock_rdlock.o pthread_rwlock_tryrdlock.o \
		pthread_rwlock_trywrlock.o pthread_rwlock_unlock.o \
		pthread_rwlock_wrlock.o

all:	${PROGS}

test1:	test1.o ${FUNCS}
		${CC} ${CFLAGS} -o $@ test1.o ${FUNCS} ${LIBS}

testcancel:	testcancel.o ${FUNCS}
		${CC} ${CFLAGS} -o $@ testcancel.o ${FUNCS} ${LIBS}

incr_rwlock1:	incr_rwlock1.o ${FUNCS}
		${CC} ${CFLAGS} -o $@ incr_rwlock1.o ${FUNCS} ${LIBS}

test2:	test2.o ${FUNCS}
		${CC} ${CFLAGS} -o $@ test2.o ${FUNCS} ${LIBS}

test3:	test3.o ${FUNCS}
		${CC} ${CFLAGS} -o $@ test3.o ${FUNCS} ${LIBS}

test4:	test4.o ${FUNCS}
		${CC} ${CFLAGS} -o $@ test4.o ${FUNCS} ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES} *.o
