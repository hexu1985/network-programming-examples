#include	"unp.h"

#define MAX_EVENTS 5 /* Maximum number of events to be returned from
                        a single epoll_wait() call */

void
str_cli(FILE *fp, int sockfd)
{
	int			    epfd, i, stdineof = 0;
    int             nready;
	char		    sendline[MAXLINE], recvline[MAXLINE];

    struct epoll_event  ev;
    struct epoll_event  evlist[MAX_EVENTS];

    epfd = Epoll_create(MAX_EVENTS);

    ev.data.fd = fileno(fp);
    ev.events = EPOLLIN;
    Epoll_ctl(epfd, EPOLL_CTL_ADD, fileno(fp), &ev);

    ev.data.fd = sockfd;
    ev.events = EPOLLIN;
    Epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);

	for ( ; ; ) {
		if (stdineof == 1)
            Epoll_ctl(epfd, EPOLL_CTL_DEL, fileno(fp), NULL);

		nready = Epoll_wait(epfd, evlist, MAX_EVENTS, -1);
        for (i = 0; i < nready; i++) {
#ifdef  NOTDEF
            printf(" fd=%d; events: %s%s%s%s%s\n", evlist[i].data.fd,
                    (evlist[i].events & EPOLLIN) ? "EPOLLIN " : "",
                    (evlist[i].events & EPOLLOUT) ? "EPOLLOUT " : "",
                    (evlist[i].events & EPOLLRDHUP) ? "EPOLLRDHUP " : "",
                    (evlist[i].events & EPOLLHUP) ? "EPOLLHUP " : "",
                    (evlist[i].events & EPOLLERR) ? "EPOLLERR " : "");
#endif

            if ((evlist[i].data.fd == sockfd) 
                    && (evlist[i].events & (EPOLLIN | EPOLLRDHUP))) {	/* socket is readable */
                if (Readline(sockfd, recvline, MAXLINE) == 0) {
                    if (stdineof == 1)
                        return;		/* normal termination */
                    else
                        err_quit("str_cli: server terminated prematurely");
                }

                Fputs(recvline, stdout);
            }

            if ((evlist[i].data.fd == fileno(fp)) 
                    && (evlist[i].events & (EPOLLIN | EPOLLRDHUP))) {  /* input is readable */
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


