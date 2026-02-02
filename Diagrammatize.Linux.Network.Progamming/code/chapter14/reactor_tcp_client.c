#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/epoll.h>
#include <pthread.h>

#define NUM (100 * 10000)
#define MAX_EVENTS (100000)
#define EPOLL_SIZE (100)
#define BUF_LEN (4096)
#define ONCE_LEN (1400)

void delay_us() {
    int i = 50 * 1000;
    while(i--);
}

int epoll_add(int efd, int fd) {
    struct epoll_event event = {0};
    event.data.fd = fd;
    event.events = EPOLLIN;
    if (epoll_ctl(efd, EPOLL_CTL_ADD, fd, &event) == -1) {
        perror("epoll ctl ADD error");
        return -1;
    }
    return 0;
}

int epoll_del(int efd, int fd) {
    if (epoll_ctl(efd, EPOLL_CTL_DEL, fd, NULL) == -1) {
        perror("epoll ctl DEL error");
        return -1;
    }
    return 0;
}

void read_handler(void *arg) {
    int fd = (int)(intptr_t)arg;
    char buf[BUF_LEN] = {0};
    int len = recv(fd, buf, ONCE_LEN, 0);
    //printf("fd:%d, len:%d, errno:%d(%s)\n", fd, len, errno, strerror(errno));
    if (len <= 0) {
        if ((errno != EAGAIN) || (len == 0)) {
            //printf("echo handler close fd:%d\n", fd);
            close(fd);
            return;
        }
    }
    //printf("fd:%d, total_len:%d\n", fd, len);
}

void* sub_reactor(void* arg) {
    int epoll_fd = (int)(intptr_t)arg;
    printf("sub reactor epoll fd:%d\n", epoll_fd);
    struct epoll_event events[MAX_EVENTS] = {0};
    int nfds;

    int timeout = 2000;
    while (1) {
        nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, timeout);
        for (int n = 0; n < nfds; ++n) {
            if (events[n].events & EPOLLIN) {
                int client_fd = events[n].data.fd;
                //printf("sub reactor epoll fd:%d, client fd:%d\n", epoll_fd, client_fd);
                read_handler((void *)(intptr_t)client_fd);
            }
        }
    }

    close(epoll_fd);
    return NULL;
}

int do_connect(const char *local_ip, const char *peer_ip, unsigned short peer_port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in local = {0};
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = inet_addr(local_ip);
    local.sin_port = htons(0);
    int ret = bind(sockfd, (struct sockaddr *)&local, sizeof(local));
    if (ret == -1) {
        perror("bind");
        close(sockfd);
        return -1;
    }

    struct sockaddr_in peer = {0};
    peer.sin_family = AF_INET;
    peer.sin_addr.s_addr = inet_addr(peer_ip);
    peer.sin_port = htons(peer_port);
    ret = connect(sockfd, (struct sockaddr *)&peer, sizeof(peer));
    if (ret == -1) {
        perror("connect");
        close(sockfd);
        return -1;
    }

    return sockfd;
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("./程序名 本机IP 服务器IP 服务器端口 连接次数(1-1000000)\n");
        return 0;
    }
    char *local_ip = argv[1];
    char *peer_ip = argv[2];
    unsigned short peer_port = atoi(argv[3]);
    int times = atoi(argv[4]);
    printf("local ip:%s, peer ip:port:%s:%u\n", local_ip, peer_ip, peer_port);


    int efd = epoll_create(EPOLL_SIZE);
    pthread_t th;
    pthread_create(&th, NULL, sub_reactor, (void*)(intptr_t)efd);

    int ret = 0;
    int fds[NUM] = {0};
    int count = 0;
    while(times--) {
        fds[count] = do_connect(local_ip, peer_ip, peer_port); 
        if (fds[count] == -1) {
            break;
        }

        epoll_add(efd, fds[count]);
        count++;
        if (count % 1000 == 0) {
            printf("client connects:%d\n", count);
        }
    }
    printf("client connects:%d\n", count);
    pause();

    char buf[BUF_LEN] = {0};
    memset(buf, 'a', BUF_LEN);
    while (1) {
        for (int i = 0; i < count; i++) {
            int ret = send(fds[i], buf, ONCE_LEN, 0);
            if (ret <= 0) {
                perror("send");
            }
            delay_us();
        }
    }

    for (int i = 0; i < count; i++) {
        close(fds[i]);
    }

    return 0;
}
