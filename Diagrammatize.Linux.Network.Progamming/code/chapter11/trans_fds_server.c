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
    memcpy(local.sun_path, UNIX_PATH, strlen(UNIX_PATH));
    int ret = bind(sockfd, (struct sockaddr *)&local, sizeof(local)); 
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

    struct msghdr msg = {0};
    struct cmsghdr *cmsg;
    char data[1024] = {0};
    char buffer[CMSG_SPACE(sizeof(int))] = {0};
    struct iovec io= {0};
    io.iov_base = data;
    io.iov_len = 1024;
    msg.msg_iov = &io;
    msg.msg_iovlen = 1;
    msg.msg_control = buffer;
    msg.msg_controllen = sizeof(buffer);
    int received_fd = -1;
    while(1) {
        ret = recvmsg(new_sockfd, &msg, 0);
        if (ret == 0) {
            printf("recvfrom ret:%d, errno:%d(%s)\n", ret, errno, strerror(errno));
            break;
        } else if ((ret == -1) && (errno != EAGAIN)) {
            printf("recvfrom ret:%d, errno:%d(%s)\n", ret, errno, strerror(errno));
            break;
        } else if ((ret == -1) && (errno == EAGAIN)) {
            continue;
        }
        printf("recvmsg data len:%d,data:%s\n", ret, msg.msg_iov->iov_base);

        cmsg = CMSG_FIRSTHDR(&msg);
        if (cmsg && cmsg->cmsg_len == CMSG_LEN(sizeof(int))) {
            if (cmsg->cmsg_level == SOL_SOCKET && cmsg->cmsg_type == SCM_RIGHTS) {
                received_fd = *(int*)CMSG_DATA(cmsg);
                printf("recevied_fd:%d\n", received_fd);
            }
        } 
        if (received_fd != -1) {
            char rbuf[1024] = {0};
            ssize_t bytes_read = read(received_fd, rbuf, 1024);
            if (bytes_read > 0) {
                printf("bytes_read:%d, rbuf:%s\n", bytes_read, rbuf);
            } else {
                perror("read");
            }
        }
        break;
    }

    close(sockfd);
    close(new_sockfd);
    close(received_fd);

    return 0;
}
