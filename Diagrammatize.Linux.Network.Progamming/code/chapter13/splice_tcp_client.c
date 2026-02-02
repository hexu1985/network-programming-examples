#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define BUF_LEN 2000
#define TEST_STR "request data"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("./a.out peer_ip peer_port\n");
        return 0;
    }
    int ret = 0;
    char *peer_ip = argv[1];
    unsigned short peer_port = atoi(argv[2]);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }

    printf("peer ip:port:%s:%u\n", peer_ip, peer_port);
    struct sockaddr_in peer = {0};
    peer.sin_family = AF_INET;
    peer.sin_addr.s_addr = inet_addr(peer_ip);
    peer.sin_port = htons(peer_port);
    ret = connect(sockfd, (struct sockaddr *)&peer, sizeof(peer));
    if (ret == -1) {
        perror("connect");
        goto out;
    }

    char rbuf[BUF_LEN] = {0};
    while(1) {
        ret = send(sockfd, TEST_STR, strlen(TEST_STR), 0); 
        if (ret <= 0) {
            perror("send");
            break;
        }

        memset(rbuf, 0, BUF_LEN);
        ret = recv(sockfd, rbuf, BUF_LEN, 0);
        if (ret <= 0) {
            perror("recv");
            break;
        }
        printf("ret:%d, rbuf:%s\n", ret, rbuf);
        sleep(1);
    }

out:
    close(sockfd);
    return 0;
}
