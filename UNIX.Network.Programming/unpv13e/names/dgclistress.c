#include	"unp.h"

#define MAX_EVENTS 5 /* Maximum number of events to be returned from
                        a single epoll_wait() call */

#define MAX_PACKETS 1000000

void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int		n;
	char	sendline[MAXLINE], recvline[MAXLINE + 1];

    int    i, epfd, nready, send_len, npkts;
    struct epoll_event  ev;
    struct epoll_event  evlist[MAX_EVENTS];

    epfd = Epoll_create(MAX_EVENTS);

    ev.data.fd = fileno(fp);
    ev.events = EPOLLIN; /* Only interested in input events */
    Epoll_ctl(epfd, EPOLL_CTL_ADD, fileno(fp), &ev);

    ev.data.fd = sockfd;
    ev.events = EPOLLIN; /* Only interested in input events */
    Epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);

    printf("entry the data to send\n");
    Fgets(sendline, MAXLINE, fp);
    send_len = strlen(sendline);
    Write(sockfd, sendline, send_len);

    npkts = 0;
	for ( ; ; ) {
		nready = Epoll_wait(epfd, evlist, MAX_EVENTS, -1);
        for (i = 0; i < nready; i++) {
#if 0
            printf(" fd=%d; events: %s%s%s%s%s\n", evlist[i].data.fd,
                    (evlist[i].events & EPOLLIN) ? "EPOLLIN " : "",
                    (evlist[i].events & EPOLLOUT) ? "EPOLLOUT " : "",
                    (evlist[i].events & EPOLLRDHUP) ? "EPOLLRDHUP " : "",
                    (evlist[i].events & EPOLLHUP) ? "EPOLLHUP " : "",
                    (evlist[i].events & EPOLLERR) ? "EPOLLERR " : "");
#endif

            if (evlist[i].data.fd == sockfd && evlist[i].events & EPOLLIN) {  /* net data in */
                n = Read(sockfd, recvline, MAXLINE);
                recvline[n] = 0;	/* null terminate */
                Write(sockfd, sendline, send_len);
                npkts++;
                if (npkts % 1000 == 0) {
                    printf("npkts=%d\n", npkts);
                }
                continue;
            }

            if (evlist[i].data.fd == fileno(fp) && evlist[i].events & EPOLLIN) {  /* stdin data in */
                if (Fgets(sendline, MAXLINE, fp) != NULL) {
                    Write(sockfd, sendline, send_len);
                    printf("npkts=%d\n", npkts);
                } else {    // EOF
                    printf("close by stdin\n");
                    return;
                }
                continue;
            }
        }
    }
}
