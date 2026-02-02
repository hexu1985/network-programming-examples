#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define BUF_LEN 1500

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

    fd_set rfds;
    fd_set rfds_bak;
    FD_ZERO(&rfds_bak);
    FD_SET(sockfd, &rfds_bak);
    int maxfd = sockfd;
    int maxfd_bak = maxfd;
    printf("maxfd_bak:%d\n", maxfd_bak);
    while(1) {
        rfds = rfds_bak;
        maxfd = maxfd_bak;
        struct timeval timeout = {.tv_sec = 2, .tv_usec = 0};
        ret = select(maxfd + 1, &rfds, NULL, NULL, NULL);
        if (ret == 0) {
            printf("select timeout\n");
            continue;
        } else if (ret == -1) {
            perror("select");
            break;
        }

        printf("timeout sec:%lu, usec:%lu\n", timeout.tv_sec, timeout.tv_usec);

        for (int i = 0; i <= maxfd; i++) {
            if (FD_ISSET(i, &rfds)) {
                if (i == sockfd) {
                    int new_sockfd = accept(i, NULL, NULL);
                    if (new_sockfd == -1) {
                        perror("accept");
                        continue;
                    }
                    FD_SET(new_sockfd, &rfds_bak);
                    maxfd_bak = new_sockfd > maxfd_bak ? new_sockfd : maxfd_bak;
                    printf("maxfd_bak:%d\n", maxfd_bak);
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
