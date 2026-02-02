#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define TEST_STRING "123456789"
int pipe_test() {
    int fd[2] = {0};
    int ret = pipe(fd);
    if (ret == -1) {
        perror("pipe");
        return -1;
    }

    ret = fork();
    if (ret == 0) {
        close(0);
        while(1) {
            write(fd[1], TEST_STRING, strlen(TEST_STRING));
            sleep(1);
        }
        close(1);
    } else if (ret > 0) {
        close(fd[1]);
        while(1) {
            char buf[1024] = {0};
            read(fd[0], buf, 1);
            printf("buf:%s\n", buf);
        }
        close(0);
    } else {
        perror("fork");
    }
    return 0;
}

int main(int argc, char *argv[]) {
    pipe_test();
    return 0;
}
