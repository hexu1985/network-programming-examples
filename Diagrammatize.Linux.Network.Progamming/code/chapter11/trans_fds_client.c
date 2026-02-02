#include <sys/types.h>
#include <sys/socket.h>
#include <linux/un.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#define UNIX_PATH "/tmp/unix_test"
#define BUF_LEN (1500)
#define REQUEST_STR "request data"
#define TEST_FILE "test.txt"
#define TEST_STRING "123456789"

int main(int argc, char *argv[]) {
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }

    struct sockaddr_un peer = {0};
    peer.sun_family = AF_UNIX;
    memcpy(peer.sun_path, UNIX_PATH, strlen(UNIX_PATH));
    int ret = connect(sockfd, (struct sockaddr *)&peer, sizeof(peer)); 
    if (ret == -1) {
        close(sockfd);
        perror("connect");
        return -1;
    }

    int file_fd = open(TEST_FILE, O_RDWR | O_CREAT | O_APPEND, 0644);
    if (file_fd == -1) {
        close(sockfd);
        perror("open");
        return -1;
    }

    struct msghdr msg = {0};
    struct cmsghdr *cmsg;
    char buffer[CMSG_SPACE(sizeof(int))] = {0};

    struct iovec io = { .iov_base = "test", .iov_len = 4 };
    msg.msg_iov  = &io;
    msg.msg_iovlen  = 1;

    msg.msg_control = buffer;
    msg.msg_controllen = sizeof(buffer);

    cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(sizeof(int));
    *(int*)CMSG_DATA(cmsg) = file_fd;
    msg.msg_controllen = cmsg->cmsg_len;

    ret = sendmsg(sockfd, &msg, 0);
    if (ret <= 0) {
        perror("sendmsg");
        close(sockfd);
        return -1;
    }
    printf("send fd:%d\n", file_fd);
    close(sockfd);
    close(file_fd);

    return 0;
}
