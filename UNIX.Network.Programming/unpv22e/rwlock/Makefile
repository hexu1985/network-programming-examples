RM = rm -f
CC = gcc
INCLUDE = -I../../lib/
CFLAGS = -Wall -g -O2 ${INCLUDE}
LIBS = ../../lib/libunpipc.a -lpthread -lrt

PROGS =	test1 testcancel incr_rwlock1 \
		test2 test3 test4

all:	${PROGS}

test1:	test1.o
		${CC} ${CFLAGS} -o $@ test1.o ${LIBS}

testcancel:	testcancel.o
		${CC} ${CFLAGS} -o $@ testcancel.o ${LIBS}

incr_rwlock1:	incr_rwlock1.o
		${CC} ${CFLAGS} -o $@ incr_rwlock1.o ${LIBS}

test2:	test2.o
		${CC} ${CFLAGS} -o $@ test2.o ${LIBS}

test3:	test3.o
		${CC} ${CFLAGS} -o $@ test3.o ${LIBS}

test4:	test4.o
		${CC} ${CFLAGS} -o $@ test4.o ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES} *.o
