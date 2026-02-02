#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define TEST_STR "123456789"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("./程序名 网络接口名或广播IP地址\n");
        return -1;
    }

    char *broadcast_ip = argv[1];
    char *if_name = argv[1];

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }

    int on = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));

#if 0
    struct sockaddr_in brd_addr = {0};
    brd_addr.sin_family = AF_INET;
    brd_addr.sin_addr.s_addr = inet_addr(broadcast_ip); 
    brd_addr.sin_port = htons(8888);
    printf("broadcast ip:%s\n", broadcast_ip); 
#else
    struct ifreq ifr = {0};
    memcpy(ifr.ifr_ifrn.ifrn_name, if_name, strlen(if_name));
    ioctl(sockfd, SIOCGIFBRDADDR, &ifr);
    struct sockaddr_in brd_addr = {0};
    memcpy(&brd_addr, &(ifr.ifr_broadaddr), sizeof(struct sockaddr));
    brd_addr.sin_port = htons(8888);
    printf("broadcast ip:%s\n", inet_ntoa(brd_addr.sin_addr));
#endif

    while(1) {
        int ret = sendto(sockfd, TEST_STR, strlen(TEST_STR), 0, (struct sockaddr *)&brd_addr, sizeof(brd_addr));  
        printf("ret:%d, error:%d(%s)\n", ret, errno, strerror(errno));
        sleep(1);
    }

    close(sockfd);

    return 0;
}
