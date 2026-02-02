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
#define LOCAL_PORT 9999
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

    struct sockaddr_in peer1 = {0};
    peer1.sin_family = AF_INET;
    peer1.sin_addr.s_addr = inet_addr(peer_ip);
    peer1.sin_port = htons(9999);

    socklen_t addrlen = sizeof(struct sockaddr_in);

#if 0
    ret = connect(sockfd, (struct sockaddr *)&peer, sizeof(peer));
    if (ret == -1) {
        perror("connect");
        return -1;
    }
#endif

    char sbuf[BUF_LEN] = {0};
    char rbuf[BUF_LEN] = {0};
    int i = 'a';
    int flags = 0;
    while(1) {
        if (i > 'a' + 4) { //发送数据包至服务端
            printf("test 111111\n");
            i = 'a';
#if 0
            //flags = 0;
            int optval = 0;
            setsockopt(sockfd, IPPROTO_UDP, UDP_CORK, &optval, sizeof(optval));
#endif
#if 1
            ret = connect(sockfd, (struct sockaddr *)&peer1, sizeof(peer1));
            if (ret == -1) {
                perror("connect");
                return -1;
            }
#endif
        } else { //开启软木塞
            printf("test 222222\n");
#if 0
            //flags = MSG_MORE;
            int optval = 1;
            setsockopt(sockfd, IPPROTO_UDP, UDP_CORK, &optval, sizeof(optval));
#endif
#if 1
            ret = connect(sockfd, (struct sockaddr *)&peer, sizeof(peer));
            if (ret == -1) {
                perror("connect");
                return -1;
            }
#endif
        }
        printf("i:%d\n", i);
        memset(sbuf, i++, sizeof(sbuf));
        int slen = 10; //sizeof(sbuf);
        ret = sendto(sockfd, sbuf, slen, flags, (struct sockaddr *)&peer, addrlen); 
        //ret = sendto(sockfd, sbuf, slen, flags, NULL, 0); 
        if (ret <= 0) {
            printf("sendto ret:%d, errno:%d(%s)\n", ret, errno, strerror(errno));
            break;
        }
        printf("sendto real len:%d\n", ret);
#if 0
        if (flags == 0) {
            for (int i = 0; i < 2; i++) {
                ret = sendto(sockfd, TEST_STR, strlen(TEST_STR), flags, (struct sockaddr *)&peer, addrlen); 
                if (ret <= 0) {
                    perror("sendto");
                    break;
                }
                printf("sendto real len:%d\n", ret);
            }
            pause();
        }
#endif

#if 0
        memset(rbuf, 0, BUF_LEN);
        struct sockaddr_in tmp;
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
