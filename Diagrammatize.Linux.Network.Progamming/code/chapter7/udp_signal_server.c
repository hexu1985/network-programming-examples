#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

#define BUF_LEN 2000

int sockfd = 0;

void set_nonblocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}

void set_async(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL);  
    fcntl(sockfd, F_SETFL, flags | O_ASYNC); 
}

void handler(int sig) {
    struct sockaddr_in peer = {0}; 
    socklen_t addr_len = sizeof(peer);
    char buf[BUF_LEN] = {0};
    int ret = recvfrom(sockfd, buf, 1400, 0, (struct sockaddr*)&peer, &addr_len);
    if (ret <= 0) {
        printf("recvfrom errno:%d(%s)\n", errno, strerror(errno));
        return;
    }
    printf("recvfrom len:%d, data:%s\n", ret, buf);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("./程序名 本机IP 服务端端口\n");
        return 0;
    }
    int ret;
    char *peer_ip = argv[1];
    unsigned short peer_port = atoi(argv[2]);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return -1;
    }

    set_nonblocking(sockfd);

    struct sockaddr_in local;
    memset(&local, 0, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_port = htons(peer_port);
    local.sin_addr.s_addr = inet_addr(peer_ip);;
    ret = bind(sockfd, (struct sockaddr *)&local, sizeof(local));
    if (ret < 0) {
        perror("bind");
        goto err;
    }

    signal(SIGIO, handler);
    fcntl(sockfd, F_SETOWN, getpid());
    set_async(sockfd);

    while (1) {
        sleep(1);
    }

err:
    close(sockfd);
    return 0;
}
