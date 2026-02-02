#ifndef __UDP_H
#define __UDP_H

#include <common.h>

struct pseudo_udphdr {
    __be32 saddr;          //源IP地址，大端模式
    __be32 daddr;         //目的IP地址，大端模式
    uint8_t fill;                   //填充0
    uint8_t protocol;       //协议，UDP协议
    __be16 len;               //UDP报文长度，大端模式
};

static inline void print_udp(const char *msg, const void *buf) {
    struct udphdr *udph = (struct udphdr *)buf;
    printf("%s\n"
            "源端口(2字节):%u\n"
            "目的端口(2字节):%u\n"
            "总长度(2字节):%u\n"
            "校验和(2字节):0x%04x\n"
            "数据:%s\n",
            msg,
            ntohs(udph->uh_sport),
            ntohs(udph->uh_dport),
            ntohs(udph->uh_ulen),
            udph->uh_sum,
            (char *)buf + sizeof(struct udphdr));
}

static inline int create_udp_pack(void *buf, unsigned short sport, unsigned short dport,
                struct sockaddr_in *saddr, struct sockaddr_in *daddr, 
                unsigned char *data, int data_len) {
    int udp_len = UDPHDR_LEN + data_len;
    struct udphdr *udph = (struct udphdr *)buf;
    udph->uh_sport = htons(sport); 
    udph->uh_dport = htons(dport);
    udph->uh_ulen = htons(udp_len);
    udph->check = 0;
    memcpy(buf + UDPHDR_LEN, data, data_len);

#if 1
    /* 计算UDP校验和 */
    char tmp[2048] = {0};
    struct pseudo_udphdr *pudph = (struct pseudo_udphdr *)tmp;
    pudph->saddr = saddr->sin_addr.s_addr;
    pudph->daddr = daddr->sin_addr.s_addr;
    pudph->fill = 0;
    pudph->protocol = IPPROTO_UDP;
    pudph->len = htons(udp_len);
    memcpy(tmp + sizeof(struct pseudo_udphdr), udph, udp_len);
    udph->check = ~checksum(tmp, sizeof(struct pseudo_udphdr) + udp_len);
#endif
    return udp_len;
}

#endif
