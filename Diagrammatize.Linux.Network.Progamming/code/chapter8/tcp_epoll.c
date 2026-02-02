#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/epoll.h>
#include <errno.h>
#include <stdlib.h>

#define MAX_FDS 1024
#define BUF_LEN 4096
#define ONCE_LEN 1500

#define IN_MODE (EPOLLIN | EPOLLET)
#define MAX_EVENTS (1024)

void set_nonblock(int fd) {
    int flags = fcntl(fd, F_GETFL);
    fcntl(fd, F_SETFL,  flags | O_NONBLOCK);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("./a.out local_ip local_port\n");
        return 0;
    }
    char *local_ip = argv[1];
    unsigned short local_port = atoi(argv[2]);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }

    set_nonblock(sockfd);

    struct sockaddr_in local = {0};
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = inet_addr(local_ip);
    local.sin_port = htons(local_port);
    int ret = bind(sockfd, (struct sockaddr *)&local, sizeof(local));
    if (ret == -1) {
        close(sockfd);
        perror("bind");
        return -1;
    }

    listen(sockfd, 5);

    int epfd = epoll_create(100);
    if (epfd == -1) {
        close(sockfd);
        perror("epoll_create");
        return -1;
    }

    struct epoll_event ev = {0};
    ev.events = IN_MODE; 
    ev.data.fd = sockfd;
    printf("init fd:%d, ADD\n", sockfd);
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);
    if (ret == -1) {
        close(sockfd);
        perror("epoll_ctl");
        return -1;
    }

    while(1) {
        struct epoll_event events[MAX_EVENTS] = {0};
        int timeout = 2000;
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout); 
        if (nfds == 0) {
            printf("epoll_wait timeout\n");
            continue;
        } else if (nfds == -1) {
            perror("epoll_wait");
            break;
        }

        //printf("nfds:%d\n", nfds);
        for (int i = 0; i < nfds; i++) {
            int fd = events[i].data.fd;
            if (events[i].events & EPOLLIN) {
                if (fd == sockfd) {
                    int new_sockfd = accept(sockfd, NULL, NULL);
                    if (new_sockfd == -1) {
                        perror("accept");
                        continue;
                    }
                    set_nonblock(new_sockfd);

                    struct epoll_event ev = {0};
                    ev.events = IN_MODE; 
                    ev.data.fd = new_sockfd;
                    epoll_ctl(epfd, EPOLL_CTL_ADD, new_sockfd, &ev);
                    printf("accept fd:%d, ADD\n", new_sockfd);
                } else {
                    char buf[BUF_LEN] = {0}; 
                    int len = 0;
                    int pos = 0;
                    while (1) {
                        pos += (len > 0 ? len : 0);
                        len = recv(fd, buf+pos, ONCE_LEN, 0); 
                        if ((len == 0) || ((len == -1) && (errno != EAGAIN))) {
                            printf("recv close fd:%d, pos:%d, DEL, errno:%d(%s)\n", fd, pos, errno, strerror(errno));
                            close(fd);
                            epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
                            break;
                        } else if ((len == -1) && (errno == EAGAIN)) {
                            //printf("fd:%d, recv len:-1, errno:EAGAIN break\n", fd);
                            break;
                        } else {
                            continue;
                        }
                    }
                    if (pos > 0) {
                        //printf("fd:%d, recv total len:%d, buf:%s\n", fd, pos, buf);
                    }
                }
            } else {
                printf("fd:%d, events:%x error", fd, events[i].events);
            }
        }
    }

    close(epfd);
    close(sockfd);
    return 0;
}
