#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define TEST_STR "video data"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("./程序名 组播IP 端口号\n");
        return -1;
    }

    char *multicast_ip = argv[1];
    unsigned short port = atoi(argv[2]);
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        printf("socket error\n");
        return -1;
    }

#if 0
    struct in_addr interface_addr;
    interface_addr.s_addr = inet_addr("192.168.1.29");
    setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_IF, (char *)&interface_addr, sizeof(interface_addr));
#endif

    struct sockaddr_in maddr = {0};
    maddr.sin_family = AF_INET;
    maddr.sin_addr.s_addr = inet_addr(multicast_ip);
    maddr.sin_port = htons(port);
    while(1) {
        sendto(sockfd, TEST_STR, strlen(TEST_STR), 0, (struct sockaddr *)&maddr, sizeof(maddr));
        sleep(1);
    }
    close(sockfd);
    return 0;
}
