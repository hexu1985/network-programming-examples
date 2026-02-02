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

#define __USE_GNU
#include <fcntl.h>

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
    
    int pipefd[2];
    ret = pipe(pipefd);
    if (ret == -1) {
        perror("pipe");
        goto err;
    }

    struct sockaddr_in peer = {0};
    socklen_t  peer_len = sizeof(peer);
    int new_sockfd = accept(sockfd, (struct sockaddr *)&peer, &peer_len);
    if (new_sockfd == -1) {
        perror("accept");
        goto err1;
    }

    printf("new socket peer ip:port:%s:%d, addrlen:%u\n", inet_ntoa(peer.sin_addr), ntohs(peer.sin_port), peer_len);
    
    while(1) {
        ret = splice(new_sockfd, NULL, pipefd[1], NULL, 32768, SPLICE_F_MOVE);
        if (ret == -1) {
            printf("splice read ret:%d, error:%d(%s)\n", ret, errno, strerror(errno));
            break;
        } else if (ret == 0) {
            printf("socket no data\n");
            continue;
        }

        ret = splice(pipefd[0], NULL, new_sockfd, NULL, 32768, SPLICE_F_MOVE);
        if (ret == -1) {
            printf("splice write ret:%d, error:%d(%s)\n", ret, errno, strerror(errno));
            break;
        } else if (ret == 0) {
            printf("pipe no data\n");
        }
    }

    close(new_sockfd);
err1:
    close(pipefd[0]);
    close(pipefd[1]);
err:
    close(sockfd);
    return 0;
}
