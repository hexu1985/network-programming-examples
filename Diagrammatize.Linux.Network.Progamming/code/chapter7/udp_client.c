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

#define BUF_LEN 2000

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("./程序名 服务端IP 服务端端口\n");
        return 0;
    }
    int ret;
    char *peer_ip = argv[1];
    unsigned short peer_port = atoi(argv[2]);
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in peer = {0};
    peer.sin_family = AF_INET;
    peer.sin_addr.s_addr = inet_addr(peer_ip);
    peer.sin_port = htons(peer_port);
    socklen_t addrlen = sizeof(struct sockaddr_in);

    char sbuf[BUF_LEN] = {0};
    int i = 'a';
    while(1) {
        memset(sbuf, i++, sizeof(sbuf));
        ret = sendto(sockfd, sbuf, 1400, 0, (struct sockaddr *)&peer, addrlen); 
        if (ret <= 0) {
            printf("sendto ret:%d, errno:%d(%s)\n", ret, errno, strerror(errno));
            break;
        }
        printf("sendto real len:%d\n", ret);
        sleep(1);
    }

    close(sockfd);
    return 0;
}
