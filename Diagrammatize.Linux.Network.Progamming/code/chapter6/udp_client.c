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
#include <pthread.h>

#define BUF_LEN 2000

void *recv_proc(void *arg) {
    int sockfd = (int)(long)arg;
    char rbuf[BUF_LEN] = {0};
    printf("recv proc sockfd:%d\n", sockfd);
    while(1) {
        struct sockaddr_in tmp;
        socklen_t addrlen = sizeof(tmp);
        memset(rbuf, 0, BUF_LEN);
        int ret = recvfrom(sockfd, rbuf, sizeof(rbuf), 0, (struct sockaddr *)&tmp, &addrlen); 
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
    }
    return NULL;
}

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

    pthread_t th;
    pthread_create(&th, NULL, recv_proc, (void *)(long)sockfd);

    struct sockaddr_in peer = {0};
    peer.sin_family = AF_INET;
    peer.sin_addr.s_addr = inet_addr(peer_ip);
    peer.sin_port = htons(peer_port);
    while(1) {
        ret = sendto(sockfd, data, strlen(data), 0, (struct sockaddr *)&peer, sizeof(peer)); 
        if (ret <= 0) {
            printf("sendto ret:%d, errno:%d(%s)\n", ret, errno, strerror(errno));
            break;
        }

        sleep(1);
    }

    close(sockfd);
    return 0;
}
