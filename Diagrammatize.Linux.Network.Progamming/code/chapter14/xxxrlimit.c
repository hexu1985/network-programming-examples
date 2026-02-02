#include <stdio.h>
#include <string.h>
#include <errno.h>

#define __USE_GNU
#include <sys/resource.h>

#define SOFT (1 * 1024 * 1024)
#define HARD (1 * 1024 * 1024)

void getrlimit_test() {
    struct rlimit old = {0};
    int ret = getrlimit(RLIMIT_NOFILE, &old);
    if (ret == -1) {
        printf("ret:%d, errno:%d(%s)\n", ret, errno, strerror(errno));
        return;
    }
    printf("rlim_cur:%lu, rlim_max:%lu\n", old.rlim_cur, old.rlim_max);
}

void setrlimit_test() {
    struct rlimit new = {.rlim_cur = SOFT, .rlim_max = HARD};
    int ret = setrlimit(RLIMIT_NOFILE, &new);
    if (ret == -1) {
        printf("ret:%d, errno:%d(%s)\n", ret, errno, strerror(errno));
        return;
    }
}

void prlimit_test() {
    struct rlimit new = {.rlim_cur = SOFT, .rlim_max = HARD};
    struct rlimit old = {0};
    int ret = prlimit(0, RLIMIT_NOFILE, &new, &old);
    if (ret == -1) {
        printf("ret:%d, errno:%d(%s)\n", ret, errno, strerror(errno));
        return;
    }
    printf("rlim_cur:%lu, rlim_max:%lu\n", old.rlim_cur, old.rlim_max);
}

int main(int argc, char *argv[]) {
    getrlimit_test();
    //setrlimit_test();
    prlimit_test();
    getrlimit_test();

    return 0;
}
