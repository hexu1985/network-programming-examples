#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/udp.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

#define BUF_LEN 2000

void set_nonblocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("./程序名 本机IP 服务端端口\n");
        return 0;
    }
    int ret;
    struct sockaddr_in local = {0};
    char *peer_ip = argv[1];
    unsigned short peer_port = atoi(argv[2]);
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }

    set_nonblocking(sockfd);

    memset(&local, 0, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = inet_addr(peer_ip);
    local.sin_port = htons(peer_port);
    ret = bind(sockfd, (struct sockaddr *)&local, sizeof(local));
    if (ret < 0) {
        perror("bind");
        goto err;
    }

    char buf[BUF_LEN] = {0};
    struct sockaddr_in peer = {0};
    socklen_t addr_len = sizeof(peer);
    while(1) {
        memset(buf, 0, BUF_LEN);
        ret = recvfrom(sockfd, buf, 1400, 0, (struct sockaddr*)&peer, &addr_len);
        if (ret <= 0) {
            if (errno == EAGAIN) {
                printf("recvfrom errno:%d(%s)\n", errno, strerror(errno));
                sleep(1);
                continue;
            } else {
                break;
            }
        }
        printf("recvfrom len:%d, data:%s\n", ret, buf);
    }

err:
    close(sockfd);
    return 0;
}
