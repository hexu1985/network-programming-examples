RM = rm -f
CC = gcc
INCLUDE = -I../../lib/
CFLAGS = -Wall -g ${INCLUDE} -DHAVE_EPOLL -DNOTDEF
LIBS = ../../lib/libunp.a -lpthread -lrt

PROGS =	test1 testga

all:	${PROGS}

test1:	test1.o
		${CC} ${CFLAGS} -o $@ test1.o ${LIBS}

testga:	testga.o
		${CC} ${CFLAGS} -o $@ testga.o ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES} *.o
