RM = rm -f
CC = gcc
INCLUDE = -I../../lib/
CFLAGS = -Wall -g -O2 ${INCLUDE}
LIBS = ../../lib/libunp.a -lpthread -lrt

PROGS =	netlink_listen

all:	${PROGS}

netlink_listen:	netlink_listen.o
		${CC} ${CFLAGS} -o $@ netlink_listen.o ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES} *.o
