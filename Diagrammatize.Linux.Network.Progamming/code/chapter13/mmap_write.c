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
#include <fcntl.h>
#include <sys/stat.h>

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
    if (once_len<= 0 || once_len > 0xffff) {
        printf("once_len:%d error\n", once_len);
        return -1;
    }
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

    int file_fd = open(pathname, O_RDONLY);
    if (file_fd < 0) {
        perror("open");
        goto out;
    }

    struct stat sb;
    if (fstat(file_fd, &sb) == -1) {
        perror("fstat");
        goto err;
    }
    printf("file size:%ld\n", sb.st_size);

    void *addr = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, file_fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        goto err;
    }

    long int total_len = 0;
    while(1) {
        int remain = sb.st_size - total_len;
        if (remain > 0 && remain < once_len) {
            once_len = remain;
        }
        ret = write(sockfd, addr + total_len, once_len);
        if (ret <= 0) {
            printf("write ret:%d, errno:%d(%s)\n", ret, errno, strerror(errno));
            goto err;
        }
        total_len += ret;
        if (total_len >= sb.st_size) {
            printf("file send total_len:%ld\n", total_len);
            break;
        }
    }
err:
    close(file_fd);
out:
    close(sockfd);
    return 0;
}
