#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <netinet/tcp.h>

#define NUM (100 * 10000)

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("./程序名 本机IP 本机端口\n");
        return 0;
    }
    char *ip = argv[1];
    unsigned short port = atoi(argv[2]);
    
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }

    int on = 1;                                  //0：关闭，1：开启
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));

    struct sockaddr_in local = {0};
    memset(&local, 0, sizeof(local));
    bzero(&local, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = inet_addr(ip);
    local.sin_port = htons(port);
    int ret = bind(sockfd, (struct sockaddr *)&local, sizeof(local));
    if (ret == -1) {
        perror("bind");
        goto err;
    }

    listen(sockfd, 5);

    int conn_count = 0;
    int new_fds[NUM] = {0};
    while(1) {
        struct sockaddr_in peer = {0};
        socklen_t  peer_len = sizeof(peer);
        new_fds[conn_count] = accept(sockfd, (struct sockaddr *)&peer, &peer_len);
        if (new_fds[conn_count] == -1) {
            perror("accept");
            break;
        }

        conn_count++;
        if (conn_count % 1000 == 0) {
            printf("total connects:%d\n", conn_count);
        }
        //printf("new socket peer ip:port:%s:%d, addrlen:%u, total connects:%d\n", inet_ntoa(peer.sin_addr), 
        //        ntohs(peer.sin_port), peer_len, conn_count);
    }
    printf("total connects:%d\n", conn_count);

    for (int i = 0; i < conn_count; i++) {
        close(new_fds[i]);
    }
    
err:
    close(sockfd);
    return 0;
}
