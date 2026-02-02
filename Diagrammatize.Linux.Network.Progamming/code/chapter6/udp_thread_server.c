#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>

#define ACK_STR "ack ok"
#define BUF_LEN 12000

void port_reuse(int sockfd) {
    int on = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));
}

int udp_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
    int ret;
    int new_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (new_sockfd == -1) {
        printf("udp accept socket error\n");
        return -1;
    }

    port_reuse(new_sockfd);

    struct sockaddr_in local;
    getsockname(sockfd, (struct sockaddr *)&local, addrlen);
    printf("udp accept local ip:port:%s:%d\n", inet_ntoa(local.sin_addr), ntohs(local.sin_port));
    ret = bind(new_sockfd, (struct sockaddr *)&local, sizeof(local));
    if (ret == -1) {
        perror("bind");
        goto err;
    }

    struct sockaddr_in *peer = (struct sockaddr_in *)addr;
    printf("peer ip:port:%s:%d, addrlen:%u\n", inet_ntoa(peer->sin_addr), ntohs(peer->sin_port), *addrlen);
    ret = connect(sockfd, (struct sockaddr *)peer, *addrlen);
    if (ret == -1) {
        perror("connect");
        goto err;
    }
    return new_sockfd;
err:
    close(new_sockfd);
    return -1;
}

void *client_proc(void *arg) {
    int sockfd = (int)(long)arg;
    printf("thread sockfd:%d\n", sockfd);
    int ret;
    struct sockaddr_in peer = {0};
    socklen_t addrlen = sizeof(peer);
    char rbuf[BUF_LEN] = {0};
    while(1) {
        memset(rbuf, 0, BUF_LEN);
        ret = recvfrom(sockfd, rbuf, sizeof(rbuf), 0, (struct sockaddr *)&peer, &addrlen); 
        if (ret <= 0) {
            perror("recvfrom");
            break;
        }
        printf("sockfd:%d, ret:%d, rbuf:%s\n", sockfd, ret, rbuf);
        ret = sendto(sockfd, ACK_STR, strlen(ACK_STR), 0, (struct sockaddr *)&peer, addrlen); 
        if (ret <= 0) {
            perror("sendto");
            break;
        }
    }

    close(sockfd);
    return NULL;
}

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

    port_reuse(sockfd);

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
    int listen_sockfd = sockfd;
    int data_sockfd;
    while(1) {
        memset(rbuf, 0, BUF_LEN);
        memset(&peer, 0, sizeof(peer));
        addrlen = sizeof(peer);
        ret = recvfrom(listen_sockfd, rbuf, sizeof(rbuf), 0, (struct sockaddr *)&peer, &addrlen); 
        if (ret <= 0) continue;
        printf("listen peer ip:port:%s:%d, addrlen:%u\n", inet_ntoa(peer.sin_addr), ntohs(peer.sin_port), addrlen);
        int new_sockfd = udp_accept(listen_sockfd, (struct sockaddr *)&peer, &addrlen);
        if (new_sockfd == -1) {
            printf("udp accept error exit\n");
            continue;
        }
        data_sockfd = listen_sockfd;
        listen_sockfd = new_sockfd;
        pthread_t th;
        int ret = pthread_create(&th, NULL, client_proc, (void*)(long)data_sockfd);
        if (ret != 0) {
            close(data_sockfd);
            continue;
        }
        pthread_detach(th);
    }
    printf("test done!\n");
    close(listen_sockfd);
    return 0;
}
