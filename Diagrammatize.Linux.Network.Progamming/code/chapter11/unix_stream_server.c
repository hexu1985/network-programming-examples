#include <sys/types.h>
#include <sys/socket.h>
#include <linux/un.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define UNIX_PATH "/tmp/unix_test"
#define BUF_LEN (1500)
#define ACK_STR "ack ok"

int main(int argc, char *argv[]) {
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }

    unlink(UNIX_PATH);

    struct sockaddr_un local = {0};
    local.sun_family = AF_UNIX;
    //memcpy(local.sun_path, UNIX_PATH, strlen(UNIX_PATH));
    //int alen = sizeof(local.sun_family);
    int alen = sizeof(local);
    int ret = bind(sockfd, (struct sockaddr *)&local, alen); 
    if (ret == -1) {
        close(sockfd);
        perror("bind");
        return -1;
    }

    struct sockaddr_un tmp = {0};
    socklen_t addrlen = sizeof(struct sockaddr_un);
    ret = getsockname(sockfd, (struct sockaddr *)&tmp, &addrlen);
    if (ret == -1) {
        close(sockfd);
        perror("getsockname");
        return -1;
    }
    printf("sun_family:%d, sun_path:%s\n", tmp.sun_family, tmp.sun_path);
    printf("0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x\n",
            tmp.sun_path[0],tmp.sun_path[1],tmp.sun_path[2],
            tmp.sun_path[3],tmp.sun_path[4],tmp.sun_path[5]);

    listen(sockfd, 5);

    int new_sockfd = accept(sockfd, NULL, NULL);
    if (new_sockfd == -1) {
        close(sockfd);
        perror("accept");
        return -1;
    }

    char rbuf[BUF_LEN] = {0};
    while(1) {
        memset(rbuf, 0, BUF_LEN);
        ret = recv(new_sockfd, rbuf, BUF_LEN, 0);
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

        ret = send(new_sockfd, ACK_STR, strlen(ACK_STR), 0);
        if (ret <= 0) {
            perror("send");
            break;
        }
    }

    close(sockfd);
    close(new_sockfd);

    return 0;
}
