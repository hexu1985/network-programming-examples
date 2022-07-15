/* include fig01 */
#include	"unp.h"

#define MAX_EVENTS 5 /* Maximum number of events to be returned from
                        a single epoll_wait() call */

int
main(int argc, char **argv)
{
	int					i, listenfd, connfd, sockfd, epfd;
	int					nready;
	ssize_t				n;
	char				buf[MAXLINE];
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddr;
    struct epoll_event  ev;
    struct epoll_event  evlist[MAX_EVENTS];

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

    epfd = Epoll_create(MAX_EVENTS);
    ev.data.fd = listenfd;
    ev.events = EPOLLIN; /* Only interested in input events */
    Epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);

/* end fig01 */

/* include fig02 */
	for ( ; ; ) {
		nready = Epoll_wait(epfd, evlist, MAX_EVENTS, -1);
        for (i = 0; i < nready; i++) {
            printf(" fd=%d; events: %s%s%s%s%s\n", evlist[i].data.fd,
                    (evlist[i].events & EPOLLIN) ? "EPOLLIN " : "",
                    (evlist[i].events & EPOLLOUT) ? "EPOLLOUT " : "",
                    (evlist[i].events & EPOLLRDHUP) ? "EPOLLRDHUP " : "",
                    (evlist[i].events & EPOLLHUP) ? "EPOLLHUP " : "",
                    (evlist[i].events & EPOLLERR) ? "EPOLLERR " : "");

            if (evlist[i].data.fd == listenfd && evlist[i].events & EPOLLIN) {  /* new client connection */
                clilen = sizeof(cliaddr);
                connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);
#ifdef	NOTDEF
                printf("new client: %s\n", Sock_ntop((SA *) &cliaddr, clilen));
#endif
                ev.data.fd = connfd;
                ev.events = EPOLLIN | EPOLLRDHUP;
                Epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
                continue;
            }

            if (evlist[i].events & (EPOLLIN | EPOLLRDHUP)) {
                sockfd = evlist[i].data.fd;
				if ( (n = read(sockfd, buf, MAXLINE)) < 0) {
					if (errno == ECONNRESET) {
							/*4connection reset by client */
#ifdef	NOTDEF
						printf("client[%d] aborted connection\n", i);
#endif
                        Epoll_ctl(epfd, EPOLL_CTL_DEL, sockfd, NULL);
						Close(sockfd);
					} else
						err_sys("read error");
				} else if (n == 0) {
						/*4connection closed by client */
#ifdef	NOTDEF
					printf("client[%d] closed connection\n", i);
#endif
                    Epoll_ctl(epfd, EPOLL_CTL_DEL, sockfd, NULL);
					Close(sockfd);
				} else
					Writen(sockfd, buf, n);
			}
        }
    }
}
/* end fig02 */
