RM = rm -f
CC = gcc
INCLUDE = -I../../lib/
CFLAGS = -Wall -g ${INCLUDE} -DHAVE_EPOLL -DNOTDEF
LIBS = ../../lib/libunp.a -lpthread -lrt

PROGS =	ssntp

all:	${PROGS}

ssntp:	main.o sntp_proc.o
		${CC} ${CFLAGS} -o $@ main.o sntp_proc.o ${LIBS} 

clean:
		rm -f ${PROGS} ${CLEANFILES} *.o
