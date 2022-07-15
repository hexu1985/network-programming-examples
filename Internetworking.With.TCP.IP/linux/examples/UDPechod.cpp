/* UDPechod.c - main, UDPechod */

#define	_USE_BSD
#include <sys/types.h>
#include <sys/signal.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <netinet/in.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define	BUFSIZE		4096

int	UDPechod(int fd);
int	errexit(const char *format, ...);
int	passiveUDP(const char *service);

/*------------------------------------------------------------------------
 * main - Concurrent UDP server for ECHO service
 *------------------------------------------------------------------------
 */
int
main(int argc, char *argv[])
{
	char	*service = "echo";	/* service name or port number	*/
	int	msock;			/* master server socket		*/
	int	ssock;			/* slave server socket		*/

	switch (argc) {
	case	1:
		break;
	case	2:
		service = argv[1];
		break;
	default:
		errexit("usage: UDPechod [port]\n");
	}

	msock = passiveUDP(service);

    UDPechod(msock);
	return 1;
}

/*------------------------------------------------------------------------
 * UDPechod - echo data until end of file
 *------------------------------------------------------------------------
 */
int
UDPechod(int fd)
{
	struct	sockaddr_in fsin;	/* the address of a client	*/
	unsigned int	alen;		/* length of client's address	*/
	char	buf[BUFSIZ];
	int	cc;

	while (true) {
        alen = sizeof(fsin);
        cc = recvfrom(fd, buf, sizeof buf, 0, (struct sockaddr *)&fsin, &alen);
        if (cc == 0)
            break;
		if (cc < 0)
			errexit("echo recvfrom: %s\n", strerror(errno));
		if (sendto(fd, buf, cc, 0, (struct sockaddr *)&fsin, alen) < 0)
			errexit("echo sendto: %s\n", strerror(errno));
	}
	return 0;
}

