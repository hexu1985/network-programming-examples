#ifndef __ETHER_H
#define __ETHER_H

#include <common.h>

static inline void print_sockaddr_ll(const char *msg, void *buf) {
    struct sockaddr_ll *sll = (struct sockaddr_ll *)buf;
    printf("%s\n"
            "sll_family:%d\n"
            "sll_protocol:%04x\n"
            "sll_ifindex:%d\n"
            "sll_hatype:%04x\n"
            "sll_pkttype:%u\n"
            "sll_halen:%u\n"
            "sll_addr[8]:%02hx:%02hx:%02hx:%02hx:%02hx:%02hx\n\n",
            msg,
            sll->sll_family,
            sll->sll_protocol,
            sll->sll_ifindex,
            sll->sll_hatype,
            sll->sll_pkttype,
            sll->sll_halen,
            sll->sll_addr[0], sll->sll_addr[1], sll->sll_addr[2], sll->sll_addr[3], sll->sll_addr[4], sll->sll_addr[5]);
}

static inline void print_ethhdr(const char *msg, void *buf) {
    struct ethhdr *eh = (struct ethhdr *)buf;
    printf("%s\n"
            "目的硬件地址：%02hx:%02hx:%02hx:%02hx:%02hx:%02hx\n"
            "源硬件地址：%02hx:%02hx:%02hx:%02hx:%02hx:%02hx\n"
            "协议：%04x\n",
            msg,
            eh->h_dest[0], eh->h_dest[1], eh->h_dest[2], eh->h_dest[3], eh->h_dest[4], eh->h_dest[5],
            eh->h_source[0], eh->h_source[1], eh->h_source[2], eh->h_source[3], eh->h_source[4], eh->h_source[5],
            ntohs(eh->h_proto));
}

static inline int create_ethhdr(void *buf, unsigned char *src_mac, 
                unsigned char *dst_mac, unsigned short proto) {
    struct ethhdr *eh = (struct ethhdr *)buf;
    memcpy(eh->h_dest, dst_mac, ETH_ALEN); 
    memcpy(eh->h_source, src_mac, ETH_ALEN); 
    eh->h_proto = htons(proto);
    return ETH_HLEN; 
}

#endif
