#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define BUF_LEN 1500

int udp_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
    int ret;
    int new_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (new_sockfd == -1) {
        printf("udp accept socket error\n");
        return -1;
    }

    int on = 1;                                  //0：关闭，1：开启
    setsockopt(new_sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    setsockopt(new_sockfd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));

    struct sockaddr_in local;
    getsockname(sockfd, (struct sockaddr *)&local, addrlen);
    printf("udp accept local ip:port:%s:%d\n", inet_ntoa(local.sin_addr), ntohs(local.sin_port));
    ret = bind(new_sockfd, (struct sockaddr *)&local, sizeof(local));
    if (ret == -1) {
        perror("bind");
        goto err;
    }

    struct sockaddr_in *peer = (struct sockaddr_in *)addr;
    printf("peer ip:port:%s:%d, addrlen:%u\n", inet_ntoa(peer->sin_addr), ntohs(peer->sin_port), *addrlen);
    ret = connect(sockfd, (struct sockaddr *)peer, *addrlen);
    if (ret == -1) {
        perror("connect");
        goto err;
    }
    return new_sockfd;
err:
    close(new_sockfd);
    return -1;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("./a.out local_ip local_port\n");
        return 0;
    }
    int ret = 0;
    char *local_ip = argv[1];
    unsigned short local_port = atoi(argv[2]);

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }

    int on = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));

    struct sockaddr_in local = {0};
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = inet_addr(local_ip);
    local.sin_port = htons(local_port);
    ret = bind(sockfd, (struct sockaddr *)&local, sizeof(local)); 
    if (ret == -1) {
        close(sockfd);
        perror("bind");
        return -1;
    }

    fd_set rfds;
    fd_set rfds_bak;
    FD_ZERO(&rfds_bak);
    FD_SET(sockfd, &rfds_bak);
    int maxfd = sockfd;
    int maxfd_bak = maxfd;
    printf("maxfd_bak:%d\n", maxfd_bak);

    char buf[BUF_LEN] = {0};
    while(1) {
        rfds = rfds_bak;
        maxfd = maxfd_bak;
        struct timeval timeout = {.tv_sec = 2, .tv_usec = 0};
        ret = select(maxfd+1, &rfds, NULL, NULL, &timeout);          
        if (ret == 0) {
            printf("select timeout maxfd_bak:%d\n", maxfd_bak);
            continue;
        } else if (ret == -1) {
            perror("select");
            break;
        }

        printf("timeout sec:%lu, usec:%lu\n", timeout.tv_sec, timeout.tv_usec);

        for (int i = 0; i <= maxfd; i++) {
            if (FD_ISSET(i, &rfds)) {
                if (i == sockfd) {
                    memset(buf, 0, sizeof(buf));
                    struct sockaddr_in peer = {0};
                    socklen_t addrlen = sizeof(peer);
                    ret = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&peer, &addrlen);
                    if (ret > 0) {
                        sockfd = udp_accept(sockfd, (struct sockaddr *)&peer, &addrlen);
                        if (sockfd == -1) {
                            printf("udp accept error exit\n");
                            continue;
                        }

                        FD_SET(sockfd, &rfds_bak);
                        maxfd_bak = sockfd > maxfd_bak ? sockfd : maxfd_bak;
                        printf("maxfd_bak:%d\n", maxfd_bak);
                    }
                } else {
                    char buf[BUF_LEN] = {0};
                    ret = recv(i, buf, BUF_LEN, 0); 
                    if ((ret == 0) || ((ret == -1) && (errno != EAGAIN))) {
                        printf("recv ret:%d, errno:%d(%s) error\n", ret, errno, strerror(errno));
                        printf("close fd:%d\n", i);
                        close(i);
                        perror("recv");
                        FD_CLR(i, &rfds_bak);
                        if (i == maxfd) {
                            for (int j = 0; j <= maxfd; j++) {
                                if (FD_ISSET(j, &rfds_bak)) {
                                    maxfd_bak = j;
                                }
                            }
                            printf("update maxfd_bak:%d\n", maxfd_bak);
                        } else {
                            printf("close fd:%d < maxfd:%d\n", i, maxfd);
                        }
                    } else if ((ret == -1) && (errno == EAGAIN)) {
                        continue;
                    } else {
                        printf("fd:%d, ret:%d, buf:%s\n", i, ret, buf);
                    }
                }
            }
        }

    }
    for (int j = 0; j <= maxfd_bak; j++) {
        if (FD_ISSET(j, &rfds_bak)) {
            close(j);
        }
    }

    return 0;
}
