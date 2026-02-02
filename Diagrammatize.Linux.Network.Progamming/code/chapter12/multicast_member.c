#include <sys/types.h>	
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("./程序名 组播IP1 组播IP2 网络接口IP 网络接口索引\n");
        return 0;
    }
    char *multicast_ip[2];
    for (int i = 0; i < 2; i++) {
        multicast_ip[i] = argv[i+1];
    }
    char *if_ip = argv[3];
    int ifindex = atoi(argv[4]);
    printf("multicast_ip1:%s, multicast ip2:%s, if_ip:%s, ifindex:%d\n", 
            multicast_ip[0], multicast_ip[1], if_ip, ifindex);
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }

    int val = 0;
    setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_ALL, &val, sizeof(val));

    struct sockaddr_in local = {0};
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    //local.sin_addr.s_addr = inet_addr("192.168.1.18");
    //local.sin_addr.s_addr = inet_addr("239.0.0.1");
    local.sin_port = htons(8888);
    int ret = bind(sockfd, (struct sockaddr *)&local, sizeof(local));
    if (ret == -1) {
        perror("bind");
        close(sockfd);
        return -1;
    }

    struct ip_mreqn mreq = {0};
    for (int i = 0; i < 2; i++) {
        memset(&mreq, 0, sizeof(mreq));
        mreq.imr_multiaddr.s_addr = inet_addr(multicast_ip[i]);
        mreq.imr_address.s_addr = htonl(INADDR_ANY); 
        //mreq.imr_address.s_addr = inet_addr(if_ip);
        //mreq.imr_ifindex = ifindex;
        ret = setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
        if (ret == -1) {
            perror("setsockopt");
            close(sockfd);
            return -1;
        }
    }

    char rbuf[2048] = {0};
    struct sockaddr_in peer = {0};
    while(1) {
        memset(rbuf, 0, 2048);
        memset(&peer, 0, sizeof(peer));
        socklen_t addrlen = sizeof(peer);
        ret = recvfrom(sockfd, rbuf, 2048, 0, (struct sockaddr *)&peer, &addrlen); 
        printf("peer ip:port:%s:%d, addrlen:%u, ret:%d, rbuf:%s\n", inet_ntoa(peer.sin_addr), ntohs(peer.sin_port), addrlen, ret, rbuf);
    }

    setsockopt(sockfd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof(mreq));

    close(sockfd);
    return 0;
}
