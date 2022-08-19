RM = rm -f
CC = gcc
INCLUDE = -I../../lib/
CFLAGS = -Wall -g -O2 ${INCLUDE} -DHAVE_EPOLL -DNOTDEF
LIBS = ../../lib/libunp.a -lpthread -lrt -lsctp

PROGS =	sctpserv01

all:	${PROGS}

sctpserv01:	sctpserv01.o sctp_getnostrm.o sctp_addr_to_associd.o sctp_wrapper.o
		${CC} ${CFLAGS} -o $@ $^ ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES} *.o
