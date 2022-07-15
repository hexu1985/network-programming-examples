/* TCPtimed.c - main */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>

int		errexit(const char *format, ...);
void		TCPtimed(int fd);
int		passiveTCP(const char *service, int qlen);

#define QLEN	32

#define	UNIXEPOCH	2208988800UL	/* UNIX epoch, in UCT secs	*/

/*------------------------------------------------------------------------
 * main - Iterative TCP server for DAYTIME service
 *------------------------------------------------------------------------
 */
int
main(int argc, char *argv[])
{
	struct	sockaddr_in fsin;	/* the from address of a client	*/
	char	*service = "daytime";	/* service name or port number	*/
	int	msock, ssock;		/* master & slave sockets	*/
	unsigned int	alen;		/* from-address length		*/

	switch (argc) {
	case	1:
		break;
	case	2:
		service = argv[1];
		break;
	default:
		errexit("usage: TCPtimed [port]\n");
	}

	msock = passiveTCP(service, QLEN);

	while (1) {
		alen = sizeof(fsin);
		ssock = accept(msock, (struct sockaddr *)&fsin, &alen);
		if (ssock < 0)
			errexit("accept failed: %s\n", strerror(errno));
		TCPtimed(ssock);
		(void) close(ssock);
	}
}

/*------------------------------------------------------------------------
 * TCPtimed - do TCP DAYTIME protocol
 *------------------------------------------------------------------------
 */
void
TCPtimed(int fd)
{
	time_t	now;			/* current time			*/

	(void) time(&now);
    now = htonl((unsigned long)(now + UNIXEPOCH));
	(void) write(fd, (char *)&now, sizeof(now));
}
