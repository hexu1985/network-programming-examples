#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#define ACK_STR "ack ok"
#define BUF_LEN 12000

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("./a.out local_ip local_port\n");
        return 0;
    }
    int ret;
    char *ip = argv[1];
    unsigned short port = atoi(argv[2]);

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }

    int on = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    struct sockaddr_in local = {0};
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = inet_addr(ip);
    local.sin_port = htons(port);
    ret = bind(sockfd, (struct sockaddr *)&local, sizeof(local));
    if (ret == -1) {
        perror("bind");
        return -1;
    }

    struct sockaddr_in peer = {0};
    socklen_t addrlen = sizeof(peer);
    char rbuf[BUF_LEN] = {0};
    while(1) {
        memset(rbuf, 0, BUF_LEN);
        memset(&peer, 0, sizeof(peer));
        addrlen = sizeof(peer);
        ret = recvfrom(sockfd, rbuf, sizeof(rbuf), 0, (struct sockaddr *)&peer, &addrlen); 
        if (ret == 0) {
            printf("recvfrom ret:%d, errno:%d(%s)\n", ret, errno, strerror(errno));
            break;
        } else if ((ret == -1) && (errno != EAGAIN)) {
            printf("recvfrom ret:%d, errno:%d(%s)\n", ret, errno, strerror(errno));
            break;
        } else if ((ret == -1) && (errno == EAGAIN)) {
            continue;
        }

        printf("peer ip:port:%s:%d, addrlen:%u\n", inet_ntoa(peer.sin_addr), ntohs(peer.sin_port), addrlen);
        printf("ret:%d, rbuf:%s\n", ret, rbuf);

        ret = sendto(sockfd, ACK_STR, strlen(ACK_STR), 0, (struct sockaddr *)&peer, addrlen); 
        if (ret <= 0) {
            perror("sendto");
            break;
        }
    }

    printf("test done!\n");

    close(sockfd);
    return 0;
}
