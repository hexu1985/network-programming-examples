#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    void *p = malloc(256);
    unsigned long *p1,*p2,*p3;
    p1 = p + 8;
    p2 = p + 2 * 8;
    p3 = p + 3 * 8;
    printf("%p\n%p\n%p\n%p\n", p, p1, p2, p3);
    printf("long:%u, int:%u\n", sizeof(long), sizeof(int));
    return 0;
}
