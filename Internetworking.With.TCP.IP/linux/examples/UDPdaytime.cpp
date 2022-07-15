/* UDPdaytime.c - main */

#include <sys/types.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>

#define	BUFSIZE 64

#define	MSG		"what time is it?\n"

int	connectUDP(const char *host, const char *service);
int	errexit(const char *format, ...);

/*------------------------------------------------------------------------
 * main - UDP client for TIME service that prints the resulting time
 *------------------------------------------------------------------------
 */
int
main(int argc, char *argv[])
{
	char	*host = "localhost";	/* host to use if none supplied	*/
	char	*service = "daytime";	/* default service name		*/
	int	s, n;			/* socket descriptor, read count*/
	char	buf[BUFSIZE];		/* buffer for one line of text	*/

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

	s = connectUDP(host, service);

	(void) write(s, MSG, strlen(MSG));

	/* Read the time */

	n = read(s, buf, sizeof(buf));
	if (n < 0)
		errexit("read failed: %s\n", strerror(errno));
    buf[n] = '\0';	/* ensure null-termination	*/
    (void) fputs(buf, stdout);
	exit(0);
}
