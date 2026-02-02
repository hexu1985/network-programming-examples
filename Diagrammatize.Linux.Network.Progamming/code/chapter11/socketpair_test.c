#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define TEST_STRING "123456789"

int main(int argc, char *argv[]) {
    int fd[2];
    int ret = socketpair(AF_UNIX, SOCK_DGRAM, 0, fd); 
    if (ret == -1) {
        perror("socketpair");
        return -1;
    }

    pid_t pid = fork();
    if (pid == 0) {
        while(1) {
            char buf[1024] = {0};
            ret = recv(fd[1], buf, 1024, 0);
            printf("child ret:%d, buf:%s\n", ret, buf);
        }
        close(fd[0]);
        close(fd[1]);
    } else if (pid > 0) {
        while(1) {
            send(fd[0], TEST_STRING, strlen(TEST_STRING), 0); 
            sleep(1);
        }
        close(fd[0]);
        close(fd[1]);
    } else {
        perror("fork");
    }

    return 0;
}
