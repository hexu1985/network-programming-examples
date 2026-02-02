#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define LOCAL_IP "192.168.1.18"
#define LOCAL_PORT (9999)
#define BUF_LEN 1500

int do_test(int argc, char *argv[]) {
    int ret = 0;
    int res = 0;
    char *peer_ip = argv[1];
    unsigned short peer_port = atoi(argv[2]);
    char *data = argv[3];

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }

    printf("peer ip:port->%s:%u\n", peer_ip, peer_port);
    struct sockaddr_in peer = {0};
    peer.sin_family = AF_INET;
    peer.sin_addr.s_addr = inet_addr(peer_ip);
    peer.sin_port = htons(peer_port);
    ret = connect(sockfd, (struct sockaddr *)&peer, sizeof(peer));
    if (ret == -1) {
        perror("connect");
        res = -1;
        goto out;
    }
    char rbuf[BUF_LEN] = {0};
    while(1) {
        ret = send(sockfd, data, strlen(data), 0); 
        if (ret <= 0) {
            perror("send");
            res = -1;
            break;
        }
        memset(rbuf, 0, BUF_LEN);
        ret = recv(sockfd, rbuf, sizeof(rbuf), 0);
        if (ret <= 0) {
            printf("recv ret:%d, errno:%d(%s)\n", ret, errno, strerror(errno));
            res = -1;
            break;
        }
        printf("ret:%d, rbuf:%s\n", ret, rbuf);
        sleep(1);
    }

out:
    close(sockfd);
    return res;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("./a.out peer_ip peer_port data\n");
        return 0;
    }

    while(1) {
        do_test(argc, argv);
        sleep(1);
    }
    return 0;
}
