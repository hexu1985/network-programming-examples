RM = rm -f
CC = gcc
INCLUDE = -I../../lib/
CFLAGS = -Wall -g -O2 ${INCLUDE}
LIBS = ../../lib/libunp.a -lpthread -lrt

OBJS = icmpd.o readable_listen.o readable_conn.o readable_v4.o readable_v6.o

PROGS =	icmpd udpcli01

all:	${PROGS}

icmpd:	${OBJS}
		${CC} ${CFLAGS} -o $@ ${OBJS} ${LIBS}

udpcli01:	udpcli01.o dgcli01.o
		${CC} ${CFLAGS} -o $@ udpcli01.o dgcli01.o ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES} *.o
