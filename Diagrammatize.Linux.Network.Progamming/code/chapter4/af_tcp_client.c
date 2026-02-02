#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define LOCAL_IP "192.168.1.18"
#define LOCAL_PORT (9999)
//#define LOCAL_IP "192.168.1.18"
//#define LOCAL_PORT (0)
#define BUF_LEN 1500
#define SEND_STR "request data"

int socket_test(int argc, char *argv[]) {

    int ret = 0;
    char *peer_ip = argv[1];
    unsigned short peer_port = atoi(argv[2]);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }

    int on = 1;                                  //0：关闭，1：开启
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));

#if 1
    struct sockaddr_in local = {0};
    bzero(&local, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = inet_addr(LOCAL_IP);
    local.sin_port = htons(LOCAL_PORT);
    ret = bind(sockfd, (struct sockaddr *)&local, sizeof(local));
    if (ret == -1) {
        close(sockfd);
        perror("bind");
        return -1;
    }
#endif

    printf("peer ip:port:%s:%u\n", peer_ip, peer_port);
    struct sockaddr_in peer = {0};
    peer.sin_family = AF_INET;
    peer.sin_addr.s_addr = inet_addr(peer_ip);
    peer.sin_port = htons(peer_port);
#if 0
    ret = connect(sockfd, (struct sockaddr *)&peer, sizeof(peer));
    if (ret == -1) {
        close(sockfd);
        perror("connect");
        return -1;
    }
#else
    ret = sendto(sockfd, SEND_STR, strlen(SEND_STR), MSG_FASTOPEN, (struct sockaddr *)&peer, sizeof(peer)); 
    if (ret == -1) {
        close(sockfd);
        perror("sendto");
        return -1;
    }
#endif

#if 0
    struct linger so_linger = {
        .l_onoff = 1,               //0：关闭延时关闭，1：开启延时关闭。
        .l_linger = 0,              //延时关闭超时时间，单位秒。
    };
    setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &so_linger, sizeof(so_linger));
#endif

    char rbuf[BUF_LEN] = {0};
    int count = 5;
    while(count--) {
        ret = send(sockfd, SEND_STR, strlen(SEND_STR), 0); 
        if (ret <= 0) {
            perror("send");
            break;
        }

#if 0
        struct sockaddr_in tmp;
        socklen_t addrlen = sizeof(struct sockaddr_in);
        ret = getsockname(sockfd, (struct sockaddr *)&tmp, &addrlen);
        printf("ret:%d, %08x\n", ret, tmp.sin_addr.s_addr);
        printf("socket local ip:port:%s:%d, addrlen:%u\n", inet_ntoa(tmp.sin_addr), ntohs(tmp.sin_port), addrlen);
#endif
#if 0
        memset(rbuf, 0, BUF_LEN);
        ret = recv(sockfd, rbuf, BUF_LEN, 0);
        if (ret <= 0) {
            perror("recv");
            break;
        }
        printf("ret:%d, rbuf:%s\n", ret, rbuf);
#endif
        sleep(1);
    }

    printf("send done!\n");
    //shutdown(sockfd, SHUT_WR);
    close(sockfd);

#if 0
    ret = send(sockfd, SEND_STR, strlen(SEND_STR), MSG_NOSIGNAL); 
    printf("ret:%d\n", ret);
    if (ret <= 0) {
        perror("send");
    }
#endif

#if 0
    while(1) {
        memset(rbuf, 0, BUF_LEN);
        ret = recv(sockfd, rbuf, BUF_LEN, 0);
        if (ret <= 0) {
            printf("recv ret:%d, errno:%d(%s)\n", ret, errno, strerror(errno));
            break;
        }
        printf("ret:%d, rbuf:%s\n", ret, rbuf);
    }
#endif

    ret = close(sockfd);
    printf("close ret:%d, errno:%d(%s)\n", ret, errno, strerror(errno));
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("./a.out peer_ip peer_port\n");
        return 0;
    }

    while (1) {
        socket_test(argc, argv);
        sleep(1);
    }

    return 0;
}
