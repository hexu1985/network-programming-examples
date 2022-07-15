/* TCPtimed.cpp - main, TCPtimed */

#include <stdlib.h>
#include <winsock.h>
#include <time.h>

void	errexit(const char *, ...);
void	TCPtimed(SOCKET);
SOCKET	passiveTCP(const char *, int);

#define QLEN	   5
#define	WSVERS	MAKEWORD(2, 0)

#define	WINEPOCH	2208988800	/* Windows epoch, in UCT secs	*/

/*------------------------------------------------------------------------
 * main - Iterative TCP server for DAYTIME service
 *------------------------------------------------------------------------
 */
int main(int argc, char *argv[])
{
	struct	sockaddr_in fsin;	/* the from address of a client	*/
	char	*service = "time";	/* service name or port number	*/
	SOCKET	msock, ssock;		/* master & slave sockets	*/
	int	alen;			/* from-address length		*/
	WSADATA wsadata;

	switch (argc) {
	case	1:
		break;
	case	2:
		service = argv[1];
		break;
	default:
		errexit("usage: TCPtimed [port]\n");
	}

	if (WSAStartup(WSVERS, &wsadata) != 0)
		errexit("WSAStartup failed\n");

	msock = passiveTCP(service, QLEN);

	while (1) {
		alen = sizeof(struct sockaddr);
		ssock = accept(msock, (struct sockaddr *)&fsin, &alen);
		if (ssock == INVALID_SOCKET)
			errexit("accept failed: error number %d\n",
				 GetLastError());
		TCPtimed(ssock);
		(void) closesocket(ssock);
	}
}

/*------------------------------------------------------------------------
 * TCPtimed - do TCP DAYTIME protocol
 *------------------------------------------------------------------------
 */
void
TCPtimed(SOCKET fd)
{
	time_t	now;			/* current time			*/

    (void) time(&now);
    now = htonl((u_long)(now + WINEPOCH));
	(void) send(fd, (char *)&now, sizeof(now), 0);
}	
