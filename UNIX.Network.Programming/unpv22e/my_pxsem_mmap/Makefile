RM = rm -f
CC = gcc
INCLUDE = -I../../lib/
CFLAGS = -Wall -g -O2 ${INCLUDE} -DHAVE_SEMAPHORE_H
LIBS = ../../lib/libunpipc.a -lpthread -lrt

PROGS =	prodcons1 testsem testeintr
FUNCS = sem_close.o sem_getvalue.o sem_open.o sem_post.o \
		sem_trywait.o sem_unlink.o sem_wait.o

all:	${PROGS}

prodcons1:	prodcons1.o ${FUNCS}
		${CC} ${CFLAGS} -o $@ prodcons1.o ${FUNCS} ${LIBS}

testsem:	testsem.o ${FUNCS}
		${CC} ${CFLAGS} -o $@ testsem.o ${FUNCS} ${LIBS}

testeintr:	testeintr.o ${FUNCS}
		${CC} ${CFLAGS} -o $@ testeintr.o ${FUNCS} ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES} *.o
