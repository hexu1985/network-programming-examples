RM = rm -f
CC = gcc
INCLUDE = -I../../lib/
CFLAGS = -Wall -g -O2 ${INCLUDE} -DHAVE_SYS_MSG_H -DHAVE_SYS_IPC_H -D_GNU_SOURCE # _GNU_SOURCE for struct msgbuf in ctl.c
LIBS = ../../lib/libunpipc.a -lpthread -lrt

PROGS =	ctl limits slot slotseq twoqueues testumask \
		msgcreate msgsnd msgrcv msgrcvid msgrmid

all:	${PROGS}

msgcreate:	msgcreate.o
		${CC} ${CFLAGS} -o $@ msgcreate.o ${LIBS}

msgsnd:	msgsnd.o
		${CC} ${CFLAGS} -o $@ msgsnd.o ${LIBS}

msgrcv:	msgrcv.o
		${CC} ${CFLAGS} -o $@ msgrcv.o ${LIBS}

msgrcvid:	msgrcvid.o
		${CC} ${CFLAGS} -o $@ msgrcvid.o ${LIBS}

msgrmid:	msgrmid.o
		${CC} ${CFLAGS} -o $@ msgrmid.o ${LIBS}

ctl:	ctl.o
		${CC} ${CFLAGS} -o $@ ctl.o ${LIBS}

limits:	limits.o
		${CC} ${CFLAGS} -o $@ limits.o ${LIBS}

slot:	slot.o
		${CC} ${CFLAGS} -o $@ slot.o ${LIBS}

slotseq:	slotseq.o
		${CC} ${CFLAGS} -o $@ slotseq.o ${LIBS}

twoqueues:	twoqueues.o
		${CC} ${CFLAGS} -o $@ twoqueues.o ${LIBS}

testumask:	testumask.o
		${CC} ${CFLAGS} -o $@ testumask.o ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES} *.o
