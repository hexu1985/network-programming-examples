#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#define NUM (100 * 10000)

int main(int argc, char *argv[]) {
    int fds[NUM] = {0};
    int count = 0;
    for (count = 0; count < NUM; count++) {
        fds[count] = socket(AF_INET, SOCK_STREAM, 0);
        if (fds[count] == -1) {
            printf("socket error:%d(%s)\n", errno, strerror(errno));
            break;
        }
    }
    printf("total socket count:%d\n", count);

    pause();

    for (int i = 0; i < count; i++) {
        close(fds[i]);
    }

    return 0;
}
