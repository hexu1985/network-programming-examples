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
    if (argc != 6) {
        printf("./a.out local_ip local_port 0或1(地址复用) 0或1(端口复用) 0或1(connect)\n");
        return 0;
    }
    int ret;
    char rbuf[BUF_LEN] = {0};
    char *ip = argv[1];
    unsigned short port = atoi(argv[2]);
    int on = atoi(argv[3]);
    int on1 = atoi(argv[4]);
    int do_connect = atoi(argv[5]);

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }
    
    printf("UDP Socket option test on:%d, on1:%d, do_connect:%d\n", on, on1, do_connect);

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &on1, sizeof(on1));

    struct sockaddr_in local = {0};
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = inet_addr(ip);
    local.sin_port = htons(port);
    ret = bind(sockfd, (struct sockaddr *)&local, sizeof(local));
    if (ret == -1) {
        perror("bind");
        return -1;
    }

    int new_sockfd;
    struct sockaddr_in peer = {0};
    socklen_t addrlen = sizeof(peer);
    int rlen = sizeof(rbuf);

    while(1) {
        memset(rbuf, 0, BUF_LEN);
        memset(&peer, 0, sizeof(peer));
        addrlen = sizeof(peer);
        rlen = 100;
        ret = recvfrom(sockfd, rbuf, rlen, 0, (struct sockaddr *)&peer, &addrlen); 
        if (ret == 0) {
            printf("recvfrom ret:%d, errno:%d(%s)\n", ret, errno, strerror(errno));
            break;
        } else if ((ret == -1) && (errno != EAGAIN)) {
            printf("recvfrom ret:%d, errno:%d(%s)\n", ret, errno, strerror(errno));
            break;
        } else if ((ret == -1) && (errno == EAGAIN)) {
            continue;
        }
        if (do_connect == 1) {
            printf("do connect++++++++++++++\n");
            connect(sockfd, (struct sockaddr *)&peer, addrlen);
            do_connect = 0;
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

    close(new_sockfd);
    close(sockfd);
    return 0;
}
