#include <sys/types.h>	
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }

    int val = 1;
    setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_ALL, &val, sizeof(val));

    struct sockaddr_in local = {0};
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    local.sin_port = htons(8888);
    int ret = bind(sockfd, (struct sockaddr *)&local, sizeof(local));
    if (ret == -1) {
        perror("bind");
        close(sockfd);
        return -1;
    }

    char rbuf[2048] = {0};
    while(1) {
        memset(rbuf, 0, 2048);
        int ret = recvfrom(sockfd, rbuf, 2048, 0, NULL, NULL);
        printf("ret:%d, rbuf:%s\n", ret, rbuf);
    }

    close(sockfd);
    return 0;
}
