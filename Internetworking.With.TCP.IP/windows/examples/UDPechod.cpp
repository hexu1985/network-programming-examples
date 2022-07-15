/* UDPechod.cpp - main, UDPechod */

#include <stdio.h>
#include <winsock.h>
#include <thread>

#define	STKSIZE		16536
#define	BUFSIZE		4096
#define	WSVERS		MAKEWORD(2, 0)

SOCKET	msock, ssock;		/* master & slave server sockets	*/

int	UDPechod(SOCKET);
void	errexit(const char *, ...);
SOCKET	passiveUDP(const char *);

/*------------------------------------------------------------------------
 * main - Concurrent UDP server for ECHO service
 *------------------------------------------------------------------------
 */
int
main(int argc, char *argv[])
{
	char	*service = "echo";	/* service name or port number	*/
	WSADATA	wsadata;

	switch (argc) {
	case	1:
		break;
	case	2:
		service = argv[1];
		break;
	default:
		errexit("usage: UDPechod [port]\n");
	}

	if (WSAStartup(WSVERS, &wsadata) != 0)
		errexit("WSAStartup failed\n");
	msock = passiveUDP(service);

    UDPechod(msock);
	WSACleanup();
	return 1;
}

/*------------------------------------------------------------------------
 * UDPechod - echo data until end of file
 *------------------------------------------------------------------------
 */
int
UDPechod(SOCKET fd)
{
	struct	sockaddr_in fsin;	/* the address of a client	*/
	int	                alen;   /* length of client's address	*/
	char	            buf[BUFSIZE];
	int	                cc;

    alen = sizeof(fsin);
	cc = recvfrom(fd, buf, sizeof buf, 0, (struct sockaddr *)&fsin, &alen);
	while (cc != SOCKET_ERROR && cc > 0) {
		if (sendto(fd, buf, cc, 0, (struct sockaddr *)&fsin, alen) == SOCKET_ERROR) {
			fprintf(stderr, "echo sendto error: %d\n",
				GetLastError());
			break;
		}
        alen = sizeof(fsin);
        cc = recvfrom(fd, buf, sizeof buf, 0, (struct sockaddr *)&fsin, &alen);
	}
	if (cc == SOCKET_ERROR)
		fprintf(stderr, "echo recvfrom error: %d\n", GetLastError());
	closesocket(fd);
	return 0;
}

