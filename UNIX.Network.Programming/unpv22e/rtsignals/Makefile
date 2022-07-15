RM = rm -f
CC = gcc
INCLUDE = -I../../lib/
CFLAGS = -Wall -g -O2 ${INCLUDE} -DHAVE_SIGINFO_T_STRUCT
LIBS = ../../lib/libunpipc.a -lpthread -lrt

PROGS =	test1 test2 test3

all:	${PROGS}

test1:	test1.o
		${CC} ${CFLAGS} -o $@ test1.o ${LIBS}

test2:	test2.o
		${CC} ${CFLAGS} -o $@ test2.o ${LIBS}

test3:	test3.o
		${CC} ${CFLAGS} -o $@ test3.o ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES} *.o
