#include	"unp.h"

void
str_cli(FILE *fp, int sockfd)
{
	int			    i, maxi, stdineof = 0;
    int             nready;
	char		    sendline[MAXLINE], recvline[MAXLINE];

    struct pollfd   fds[2];
    fds[0].fd = fileno(fp);
    fds[0].events = POLLIN;
    fds[1].fd = sockfd;
    fds[1].events = POLLRDNORM;
    maxi = 1;

	for ( ; ; ) {
		if (stdineof == 1)
            fds[0].fd = -1;

		nready = Poll(fds, maxi+1, INFTIM);

        for (i = 0; i <= maxi; i++) {
            if (fds[i].fd < 0)
                continue;

            if ((fds[i].fd == sockfd) 
                    && (fds[i].revents & (POLLRDNORM | POLLERR))) {	/* socket is readable */
                if (Readline(sockfd, recvline, MAXLINE) == 0) {
                    if (stdineof == 1)
                        return;		/* normal termination */
                    else
                        err_quit("str_cli: server terminated prematurely");
                }

                Fputs(recvline, stdout);
            }

            if ((fds[i].fd == fileno(fp)) 
                    && (fds[i].revents & (POLLIN | POLLERR))) {  /* input is readable */
                if (Fgets(sendline, MAXLINE, fp) == NULL) {
                    stdineof = 1;
                    Shutdown(sockfd, SHUT_WR);	/* send FIN */
                    continue;
                }

                Writen(sockfd, sendline, strlen(sendline));
            }
        }
	}
}

