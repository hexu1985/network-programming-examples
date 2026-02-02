#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define NUM (100 * 10000)

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

    int ret = 0;
    int fds[NUM] = {0};
    int count = 0;
    while(times--) {
        fds[count] = do_connect(local_ip, peer_ip, peer_port); 
        if (fds[count] == -1) {
            break;
        }
        count++;
        if (count % 1000 == 0) {
            printf("client connects:%d\n", count);
        }
        //usleep(1000);
    }
    printf("client connects:%d\n", count);
    pause();

    for (int i = 0; i < count; i++) {
        close(fds[i]);
    }

    return 0;
}
