#ifndef __IP_H
#define __IP_H

#include <common.h>

static inline void print_iphdr(const char *msg, const void *buf) {
    struct iphdr *iph = (struct iphdr *)buf;
    char src_ip[16] = {0}; char dst_ip[16] = {0};
    inet_ntop(AF_INET, &iph->saddr, src_ip, 16);
    inet_ntop(AF_INET, &iph->daddr, dst_ip, 16);
    printf("%s\n"
            "版本号(4位):%u\n"
            "头部长度(4位):%u\n"
            "服务类型(8位):%u\n"
            "总长度(16位):%u\n"
            "标识(16位):%u\n"
            "3位标志+ 13为片偏移(16位):0x%04x\n"
            "生存时间(8位):%u\n"
            "协议号(8位):%u\n"
            "头部校验和(16位):0x%x\n"
            "源IP地址:%s\n"
            "目的IP地址:%s\n",
            msg, iph->version, iph->ihl, iph->tos, ntohs(iph->tot_len),
            ntohs(iph->id), ntohs(iph->frag_off),
            iph->ttl, iph->protocol, iph->check,
            src_ip,
            dst_ip);
}

static inline int create_iphdr(void *buf, struct sockaddr_in *saddr, struct sockaddr_in *daddr, unsigned char protocol, unsigned int data_len) {
    struct iphdr *iph = (struct iphdr *)buf; 
    iph->version = IPVERSION;
    iph->ihl = 5;
    iph->tos = 0;
    iph->tot_len = htons(IPHDR_LEN + data_len);
    iph->id = htons(99);
    iph->frag_off = htons(IP_DF);
    iph->ttl = MAXTTL;
    iph->protocol = protocol;
    iph->saddr = saddr->sin_addr.s_addr;
    iph->daddr = daddr->sin_addr.s_addr; 
    iph->check = 0;
    iph->check = ~checksum(iph, IPHDR_LEN);

    return IPHDR_LEN; 
}

#endif
