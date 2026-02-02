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

#define TEST_STR "test str"
#define BUF_LEN 2000

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("./a.out peer_ip peer_port\n");
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

#if 0
    struct sockaddr_in local = {0};
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = inet_addr(LOCAL_IP);
    local.sin_port = htons(LOCAL_PORT);
    ret = bind(sockfd, (struct sockaddr *)&local, sizeof(local));
    if (ret == -1) {
        perror("bind");
        return -1;
    }
#endif

    struct sockaddr_in peer = {0};
    peer.sin_family = AF_INET;
    peer.sin_addr.s_addr = inet_addr(peer_ip);
    peer.sin_port = htons(peer_port);
    socklen_t addrlen = sizeof(peer);

#if 0
    ret = connect(sockfd, (struct sockaddr *)&peer, sizeof(peer));
    if (ret == -1) {
        perror("connect");
        return -1;
    }
#endif

    int flags = 0;
    while(1) {
        ret = sendto(sockfd, TEST_STR, sizeof(TEST_STR), flags, (struct sockaddr *)&peer, addrlen); 
        if (ret <= 0) {
            printf("sendto ret:%d, errno:%d(%s)\n", ret, errno, strerror(errno));
            break;
        }
        //printf("sendto real len:%d\n", ret);

        sleep(1);
    }

    close(sockfd);
    return 0;
}
