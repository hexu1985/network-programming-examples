RM = rm -f
CC = gcc
INCLUDE = -I../../lib/
CFLAGS = -Wall -g -O2 ${INCLUDE} -DHAVE_SYS_IPC_H
LIBS = ../../lib/libunpipc.a -lpthread -lrt

PROGS =	ftok ftok1 

all:	${PROGS}

ftok:	ftok.o
		${CC} ${CFLAGS} -o $@ ftok.o ${LIBS}

ftok1:	ftok1.o
		${CC} ${CFLAGS} -o $@ ftok1.o ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES} *.o
