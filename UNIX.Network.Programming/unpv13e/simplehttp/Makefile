RM = rm -f
CC = gcc
INCLUDE = -I../../lib/
CFLAGS = -Wall -g ${INCLUDE} -DHAVE_EPOLL -DNOTDEF
LIBS = ../../lib/libunp.a -lpthread -lrt

PROGS =	httpcli01

all:	${PROGS}

httpcli01:	httpcli01.o
		${CC} ${CFLAGS} -o $@ httpcli01.o ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES} *.o
