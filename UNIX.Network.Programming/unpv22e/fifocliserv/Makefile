RM = rm -f
CC = gcc
INCLUDE = -I../../lib/
CFLAGS = -Wall -g -O2 ${INCLUDE}
LIBS = ../../lib/libunpipc.a -lpthread -lrt

PROGS =	mainclient mainserver

all:	${PROGS}

mainclient:	mainclient.o
		${CC} ${CFLAGS} -o $@ mainclient.o ${LIBS}

mainserver:	mainserver.o
		${CC} ${CFLAGS} -o $@ mainserver.o ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES} *.o
