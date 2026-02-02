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

#define LOCAL_IP "192.168.1.18"
#define LOCAL_PORT 7777
#define TEST_STR "helloworld"
#define BUF_LEN 2000

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("./a.out peer_ip peer_port data\n");
        return 0;
    }
    int ret;
    char *peer_ip = argv[1];
    unsigned short peer_port = atoi(argv[2]);
    char *data = argv[3];
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }

    int on = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    int sbuf_size = 0;
    int rbuf_size = 0;
    int len = sizeof(int);
    sbuf_size = 20000; //8 * 1024 * 1024;
    setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &sbuf_size, len);
    getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &sbuf_size, &len);
    printf("sbuf_size:%d, rbuf_size:%d\n", sbuf_size, rbuf_size);

    struct sockaddr_in local = {0};
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = inet_addr(LOCAL_IP);
    local.sin_port = htons(LOCAL_PORT);
    ret = bind(sockfd, (struct sockaddr *)&local, sizeof(local));
    if (ret == -1) {
        perror("bind");
        return -1;
    }

    struct sockaddr_in peer = {0};
    peer.sin_family = AF_INET;
    peer.sin_addr.s_addr = inet_addr(peer_ip);
    peer.sin_port = htons(peer_port);

    char rbuf[BUF_LEN] = {0};
    while(1) {
        ret = sendto(sockfd, data, strlen(data), 0, (struct sockaddr *)&peer, sizeof(peer)); 
        if (ret <= 0) {
            printf("sendto ret:%d, errno:%d(%s)\n", ret, errno, strerror(errno));
            break;
        }
        printf("sendto real len:%d\n", ret);
#if 0
        memset(rbuf, 0, BUF_LEN);
        struct sockaddr_in tmp;
        socklen_t addrlen = sizeof(tmp);
        ret = recvfrom(sockfd, rbuf, BUF_LEN, 0, (struct sockaddr *)&tmp, &addrlen); 
        if (ret == 0) {
            printf("recvfrom ret:%d, errno:%d(%s)\n", ret, errno, strerror(errno));
            break;
        } else if ((ret == -1) && (errno != EAGAIN)) {
            printf("recvfrom ret:%d, errno:%d(%s)\n", ret, errno, strerror(errno));
            break;
        } else if ((ret == -1) && (errno == EAGAIN)) {
            continue;
        }
        printf("tmp ip:port:%s:%d, addrlen:%u\n", inet_ntoa(tmp.sin_addr), ntohs(tmp.sin_port), addrlen);
        printf("ret:%d, rbuf:%s\n", ret, rbuf);
#endif

        sleep(1);
    }

    close(sockfd);
    return 0;
}
