#include <stdio.h>
#include <endian.h>
#include <stdint.h>

void show_endian() {
    union{
        short a;
        char b;
    } num = {0};    //联合体各成员共享内存空间
    num.a = 0x01;  //可以把0x01看成0x0001
    if (num.b == 0x01) {
        printf("little endian\n");
    } else {
        printf("big endian\n");
    }
}

int main(int argc, char *argv[]) {
    uint64_t num = 0x1234567812345678;
    uint64_t num1 = htole64(num);
    printf("num:%016llX, num1:%016llX\n", num, num1);
    show_endian();
    
    return 0;
}
