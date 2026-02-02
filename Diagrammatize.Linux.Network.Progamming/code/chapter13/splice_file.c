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
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/sendfile.h>

#define __USE_GNU
#include <fcntl.h>

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("./程序名 服务器IP 服务器端口 文件名 单次写数据长度(0-65536)\n");
        return 0;
    }
    int ret;
    char *peer_ip = argv[1];
    unsigned short peer_port = atoi(argv[2]);
    char *pathname = argv[3];
    int once_len = atoi(argv[4]); 
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in peer = {0};
    peer.sin_family = AF_INET;
    peer.sin_addr.s_addr = inet_addr(peer_ip);
    peer.sin_port = htons(peer_port);
    ret = connect(sockfd, (struct sockaddr *)&peer, sizeof(peer));
    if (ret == -1) {
        perror("connect");
        goto out;
    }

    int filefd = open(pathname, O_RDONLY);
    if (filefd < 0) {
        perror("open");
        goto out;
    }

    struct stat sb;
    if (fstat(filefd, &sb) == -1) {
        perror("fstat");
        goto err;
    }
    printf("file size:%d\n", sb.st_size);

    int pipefd[2];
    ret = pipe(pipefd);
    if (ret == -1) {
        perror("pipe");
        goto err;
    }

    int total_len = 0;
    while(1) {
        ret = splice(filefd, NULL, pipefd[1], NULL, once_len, SPLICE_F_MOVE);
        if (ret == -1) {
            printf("splice read file ret:%d, error:%d(%s)\n", ret, errno, strerror(errno));
            break;
        } else if (ret == 0) {
            //printf("socket no data\n");
            continue;
        }
        total_len += ret;
        printf("total_len:%d\n", total_len);

        ret = splice(pipefd[0], NULL, sockfd, NULL, ret, SPLICE_F_MOVE);
        if (ret == -1) {
            printf("splice send data ret:%d, error:%d(%s)\n", ret, errno, strerror(errno));
            break;
        } else if (ret == 0) {
            //printf("pipe no data\n");
        }
    }
    close(pipefd[0]);
    close(pipefd[1]);
err:
    close(filefd);
out:
    close(sockfd);
    return 0;
}
