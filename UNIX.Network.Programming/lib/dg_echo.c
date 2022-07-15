#include	"dg_echo.h"

void
dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen)
{
	int			n;
	socklen_t	len;
	char		mesg[MAXLINE];

#ifdef	NOTDEF
    char                str[INET_ADDRSTRLEN];
#endif

	for ( ; ; ) {
		len = clilen;
		n = Recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len);

#ifdef	NOTDEF
        printf("recvfrom: %s, len: %d\n", Sock_ntop(pcliaddr, clilen), n);
#endif

		Sendto(sockfd, mesg, n, 0, pcliaddr, len);
	}
}
