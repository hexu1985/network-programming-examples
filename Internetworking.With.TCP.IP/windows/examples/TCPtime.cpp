/* TCPtime.cpp - main, TCPtime */

#include <stdlib.h>
#include <stdio.h>
#include <winsock.h>
#include <time.h>

void	TCPtime(const char *, const char *);
void	errexit(const char *, ...);
SOCKET	connectTCP(const char *, const char *);

#define	LINELEN		 128
#define WSVERS		MAKEWORD(2, 0)

#define	WINEPOCH	2208988800	/* Windows epoch, in UCT secs	*/

/*------------------------------------------------------------------------
 * main - TCP client for DAYTIME service
 *------------------------------------------------------------------------
 */
int
main(int argc, char *argv[])
{
	char	*host = "localhost";	/* host to use if none supplied	*/
	char	*service = "time";	    /* default service port		*/
	WSADATA wsadata;

	switch (argc) {
	case 1:
		host = "localhost";
		break;
	case 3:
		service = argv[2];
		/* FALL THROUGH */
	case 2:
		host = argv[1];
		break;
	default:
		fprintf(stderr, "usage: TCPtime [host [port]]\n");
		exit(1);
	}

	if (WSAStartup(WSVERS, &wsadata) != 0)
		errexit("WSAStartup failed\n");
	TCPtime(host, service);
	WSACleanup();
	return 0;	/* exit */
}

/*------------------------------------------------------------------------
 * TCPtime - invoke Daytime on specified host and print results
 *------------------------------------------------------------------------
 */
void
TCPtime(const char *host, const char *service)
{
	char	buf[LINELEN+1];		/* buffer for one line of text	*/
	SOCKET	s;			/* socket descriptor		*/
	time_t	now;			/* 32-bit integer to hold time	*/
	int	n;			/* recv character count		*/

  	s = connectTCP(host, service);

	n = recv(s, (char *)&now, sizeof(now), 0);
  	if (n == SOCKET_ERROR)
		errexit("recv failed: recv() error %d\n", GetLastError());
	now = ntohl((u_long)now);	/* put in host byte order	*/
	now -= WINEPOCH;		/* convert UCT to Windows epoch	*/
	printf("%s", ctime(&now));
	closesocket(s);
}
