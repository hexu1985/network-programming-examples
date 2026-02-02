#include <sys/types.h>
#include <sys/socket.h>
#include <linux/un.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define LOCAL_PATH "/tmp/server"
#define BUF_LEN 1500
#define ACK_STR "ack ok"

int main(int argc, char *argv[]) {
    int sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }

    unlink(LOCAL_PATH);

    struct sockaddr_un local = {0};
    local.sun_family = AF_UNIX;
    memcpy(local.sun_path, LOCAL_PATH, strlen(LOCAL_PATH));
    int ret = bind(sockfd, (struct sockaddr *)&local, sizeof(local));
    if (ret == -1) {
        close(sockfd);
        perror("bind");
        return -1;
    }

    char rbuf[BUF_LEN] = {0};
    while(1) {
        memset(rbuf, 0, BUF_LEN);
        struct sockaddr_un peer = {0};
        socklen_t addrlen = sizeof(peer);
        ret = recvfrom(sockfd, rbuf, BUF_LEN, 0, (struct sockaddr *)&peer, &addrlen); 
        if (ret == 0) {
            printf("recvfrom ret:%d, errno:%d(%s)\n", ret, errno, strerror(errno));
            break;
        } else if ((ret == -1) && (errno != EAGAIN)) {
            printf("recvfrom ret:%d, errno:%d(%s)\n", ret, errno, strerror(errno));
            break;
        } else if ((ret == -1) && (errno == EAGAIN)) {
            continue;
        }
        printf("len:%d, addr path:%s, rbuf:%s\n", ret, peer.sun_path, rbuf);

        ret = sendto(sockfd, ACK_STR, strlen(ACK_STR), 0, (struct sockaddr *)&peer, addrlen); 
        if (ret <= 0) {
            perror("sendto");
            break;
        }
    }

    close(sockfd);

    return 0;
}
