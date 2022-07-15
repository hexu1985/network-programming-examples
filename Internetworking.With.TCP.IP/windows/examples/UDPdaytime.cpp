/* UDPdaytime.cpp - main */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <winsock.h>

#define	BUFSIZE		  64
#define	WSVERS		MAKEWORD(2, 0)

#define	MSG		"what time is it?\n"

SOCKET	connectUDP(const char *, const char *);
void	errexit(const char *, ...);

/*------------------------------------------------------------------------
 * main - UDP client for TIME service that prints the resulting time
 *------------------------------------------------------------------------
 */
int
main(int argc, char *argv[])
{
	char	*host = "localhost";	/* host to use if none supplied	*/
	char	*service = "daytime";	/* default service name		*/
	SOCKET	s;			/* socket descriptor		*/
	int	n;			/* recv count			*/
	char	buf[BUFSIZE];		/* buffer for one line of text	*/
	WSADATA	wsadata;

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
		fprintf(stderr, "usage: UDPtime [host [port]]\n");
		exit(1);
	}
	if (WSAStartup(WSVERS, &wsadata))
		errexit("WSAStartup failed\n");

	s = connectUDP(host, service);

	(void) send(s, MSG, strlen(MSG), 0);

	/* Read the time */

	n = recv(s, buf, sizeof(buf), 0);
  	if (n == SOCKET_ERROR)
		errexit("recv failed: recv() error %d\n", GetLastError());
    buf[n] = '\0';	/* ensure null-termination	*/
    (void) fputs(buf, stdout);
	WSACleanup();
	return 0;	/* exit */
}
