#include <sys/types.h>
#include <sys/socket.h>
#include <linux/un.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define UNIX_PATH "/tmp/unix_test"
#define BUF_LEN (1500)
#define REQUEST_STR "request data"

int main(int argc, char *argv[]) {
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }

    struct sockaddr_un peer = {0};
    peer.sun_family = AF_UNIX;
    //memcpy(peer.sun_path, UNIX_PATH, strlen(UNIX_PATH));
    //memcpy(peer.sun_path, "\x00\x61\x30\x31\x62\x36", 6);
    int ret = connect(sockfd, (struct sockaddr *)&peer, sizeof(peer)); 
    if (ret == -1) {
        close(sockfd);
        perror("connect");
        return -1;
    }

    char rbuf[BUF_LEN] = {0};
    while(1) {
        ret = send(sockfd, REQUEST_STR, strlen(REQUEST_STR), 0);
        if (ret <= 0) {
            perror("send");
            break;
        }

        memset(rbuf, 0, BUF_LEN);
        ret = recv(sockfd, rbuf, BUF_LEN, 0);
        if (ret == 0) {
            printf("recvfrom ret:%d, errno:%d(%s)\n", ret, errno, strerror(errno));
            break;
        } else if ((ret == -1) && (errno != EAGAIN)) {
            printf("recvfrom ret:%d, errno:%d(%s)\n", ret, errno, strerror(errno));
            break;
        } else if ((ret == -1) && (errno == EAGAIN)) {
            continue;
        }
        printf("ret:%d, rbuf:%s\n", ret, rbuf);

        sleep(1);
    }

    close(sockfd);

    return 0;
}
