RM = rm -f
CXX = g++
INCLUDE = -I../common
CXXFLAGS = -Wall -g -O2 ${INCLUDE} -D_WIN32_WINNT=0x0600	# _WIN32_WINNT for inet_ntop
LIBS = -lwsock32 -lws2_32
VPATH = 

PROGS =	TCPecho TCPdaytime TCPtecho UDPecho UDPtime \
		TCPdaytimed TCPechod TCPmechod UDPtimed daytimed \
		superd UDPechod UDPdaytime UDPdaytimed \
		TCPtime TCPtimed

all:	${PROGS}

TCPecho: TCPecho.o connectTCP.o connectsock.o errexit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

TCPdaytime: TCPdaytime.o connectTCP.o connectsock.o errexit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

TCPtecho: TCPtecho.o connectTCP.o connectsock.o errexit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

UDPecho: UDPecho.o connectUDP.o connectsock.o errexit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

UDPtime: UDPtime.o connectUDP.o connectsock.o errexit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

UDPdaytime: UDPdaytime.o connectUDP.o connectsock.o errexit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

TCPdaytimed: TCPdaytimed.o passiveTCP.o passivesock.o errexit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

TCPechod: TCPechod.o passiveTCP.o passivesock.o errexit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

TCPmechod: TCPmechod.o passiveTCP.o passivesock.o errexit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

TCPtime: TCPtime.o connectTCP.o connectsock.o errexit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

TCPtimed: TCPtimed.o passiveTCP.o passivesock.o errexit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

UDPechod: UDPechod.o passiveUDP.o passivesock.o errexit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

UDPdaytimed: UDPdaytimed.o passiveUDP.o passivesock.o errexit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

UDPtimed: UDPtimed.o passiveUDP.o passivesock.o errexit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

daytimed: daytimed.o passiveTCP.o passiveUDP.o passivesock.o errexit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

superd: superd.o sv_funcs.o passiveTCP.o passiveUDP.o passivesock.o errexit.o
		${CXX} ${CXXFLAGS} -o $@ $^ ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES} *.o

distclean:
		rm -f ${PROGS} ${CLEANFILES} *.o
