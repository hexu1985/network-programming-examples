#ifndef __COMMON_H
#define __COMMON_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <linux/if_arp.h>
#include <linux/if_ether.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <errno.h>
#include <stdbool.h>

#define IPv4_LEN (4)
#define IPHDR_LEN (20)
#define UDPHDR_LEN (8)

static inline int get_if_mac(const char *if_name, void *mac) {
    struct ifreq ifr;
    int fd;

    bzero(&ifr, sizeof(struct ifreq));
    if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;

    strncpy(ifr.ifr_name, if_name, sizeof(ifr.ifr_name) - 1);
    if(ioctl(fd, SIOCGIFHWADDR, &ifr) < 0) {
        close(fd);
        return -1;
    }
    memcpy(mac, ifr.ifr_hwaddr.sa_data, ETH_ALEN);
    close(fd);
    return 0;
}

static inline int get_dst_mac(const char *if_name, char *ip_addr, void *mac) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }

    struct arpreq arpreq = {0};
    struct sockaddr_in *sin = (struct sockaddr_in *)&arpreq.arp_pa;
    sin->sin_family = AF_INET;
    sin->sin_addr.s_addr = inet_addr(ip_addr);
    strncpy(arpreq.arp_dev, if_name, strlen(if_name));

    if (ioctl(sockfd, SIOCGARP, &arpreq) < 0) {
        perror("Failed to get ARP entry");
        close(sockfd);
        return -1;
    }

    memcpy(mac, arpreq.arp_ha.sa_data, ETH_ALEN);

    close(sockfd);
    return 0;
}

static inline int get_if_index(const char *if_name, int *ifindex) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct ifreq req = {0};
    memcpy(req.ifr_ifrn.ifrn_name, if_name, strlen(if_name));
    int ret = ioctl(sockfd, SIOCGIFINDEX, &req);
    if (ret == -1) {
        perror("ioctl");
        return -1;
    }
    *ifindex = req.ifr_ifru.ifru_ivalue;
    close(sockfd);
    return 0;
}

static int get_if_ip(const char *if_name, void *addr) {
    struct ifreq ifr;
    int fd;

    bzero(&ifr, sizeof(struct ifreq));
    if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;

    strncpy(ifr.ifr_name, if_name, sizeof(ifr.ifr_name) - 1);
    if(ioctl(fd, SIOCGIFADDR, &ifr) < 0) {
        close(fd);
        return -1;
    }
    memcpy(addr, &ifr.ifr_addr, sizeof(struct sockaddr));
    close(fd);
    return 0;
}


static inline uint16_t checksum(void *buf, int size) {
    register uint32_t sum = 0;
    uint16_t *tmp_buf = (uint16_t *)buf;
    while(size > 1) {
        sum += *tmp_buf++;
        size -= 2;
    }
    while(sum >> 16) { 
        sum = (sum >> 16) + (sum & 0xffff);
    }
    return (uint16_t)(sum & 0xffff);
}

#endif
