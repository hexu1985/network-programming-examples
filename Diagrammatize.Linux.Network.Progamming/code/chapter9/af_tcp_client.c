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
#define SEND_STR "request data"

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("./a.out peer_ip peer_port 0或1(地址复用) 0或1(端口复用)\n");
        return 0;
    }

    char *peer_ip = argv[1];
    unsigned short peer_port = atoi(argv[2]);
    int on = atoi(argv[3]);
    int on1 = atoi(argv[4]);

    int ret;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &on1, sizeof(on1));

    int on2 = 1;                              //0：关闭保活，1：开启保活
    setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &on2, sizeof(on2));
    int idle = 10;                            //10秒没数据交互发送保活探测报文
    setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPIDLE, &idle, sizeof(idle));
    int intvl = 3;                            //保活探测报文发送周期为3秒
    setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPINTVL, &intvl, sizeof(intvl));
    int cnt = 3;                              //总共发送3个保活探测报文
    setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPCNT, &cnt, sizeof(cnt));

    int opt = 1;
    ret = setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));
    if (ret == -1) {
        perror("set TCP_NODELAY error");
        close(sockfd);
        return -1;
    }
    opt = 1;
    ret = setsockopt(sockfd, IPPROTO_TCP, TCP_CORK, &opt, sizeof(opt));
    if (ret == -1) {
        perror("set TCP_CORK error");
        close(sockfd);
        return -1;
    }
    int ropt = -1;
    socklen_t optlen = sizeof(ropt);
    getsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &ropt, &optlen);
    printf("ropt:%d\n", ropt);
    ropt = -1;
    getsockopt(sockfd, IPPROTO_TCP, TCP_CORK, &ropt, &optlen);
    printf("ropt:%d\n", ropt);
    //setsockopt(sockfd, IPPROTO_TCP, TCP_CORK, &opt1, sizeof(opt1));

#if 0
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

    printf("peer ip:port->%s:%u\n", peer_ip, peer_port);
    struct sockaddr_in peer = {0};
    peer.sin_family = AF_INET;
    peer.sin_addr.s_addr = inet_addr(peer_ip);
    peer.sin_port = htons(peer_port);
    ret = connect(sockfd, (struct sockaddr *)&peer, sizeof(peer));
    if (ret == -1) {
        close(sockfd);
        perror("connect");
        return -1;
    }

    char rbuf[BUF_LEN] = {0};
    int count = 0;
    while(1) {
        if (count < 100) {
            ret = send(sockfd, SEND_STR, strlen(SEND_STR), 0); 
            if (ret <= 0) {
                perror("send");
                break;
            }
        }
        count++;
        if (count > 100) pause();
        usleep(1000);
    }

    printf("send done!\n");
    close(sockfd);
    return 0;
}
