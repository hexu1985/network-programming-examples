RM = rm -f
CC = gcc
INCLUDE = -I../../lib/
CFLAGS = -Wall -g ${INCLUDE} -DHAVE_EPOLL -DNOTDEF
LIBS = ../../lib/libunp.a -lpthread -lrt

PROGS =	mysdr

all:	${PROGS}

mysdr:	main.o loop.o
		${CC} ${CFLAGS} -o $@ main.o loop.o ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES} *.o
