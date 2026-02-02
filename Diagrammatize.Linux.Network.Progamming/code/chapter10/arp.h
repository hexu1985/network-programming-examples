#ifndef __ARP_H
#define __ARP_H

#include <common.h>

struct arppack {
	__be16		ar_hrd;
	__be16		ar_pro;
	unsigned char	ar_hln;
	unsigned char	ar_pln;
	__be16		ar_op;

	unsigned char		ar_sha[ETH_ALEN];
	unsigned char		ar_sip[IPv4_LEN];
	unsigned char		ar_tha[ETH_ALEN];
	unsigned char		ar_tip[IPv4_LEN];
};

static inline void print_arp(const char *msg, const void *buf) {
    struct arppack *ah = (struct arppack *)buf;
    printf("%s\n"
            "硬件类型：%u\n"
            "协议类型：%u\n"
            "硬件地址长度：%u\n"
            "协议地址长度：%u\n"
            "操作码：%u\n"
            "发送方硬件地址：%02hx:%02hx:%02hx:%02hx:%02hx:%02hx\n"
            "发送方协议地址：%d.%d.%d.%d\n"
            "接收方硬件地址：%02hx:%02hx:%02hx:%02hx:%02hx:%02hx\n"
            "接收方协议地址：%d.%d.%d.%d\n",
            msg,
            ntohs(ah->ar_hrd),
            ntohs(ah->ar_pro),
            ah->ar_hln,
            ah->ar_pln,
            ntohs(ah->ar_op),
            ah->ar_sha[0], ah->ar_sha[1], ah->ar_sha[2], ah->ar_sha[3], ah->ar_sha[4], ah->ar_sha[5],
            ah->ar_sip[0], ah->ar_sip[1], ah->ar_sip[2], ah->ar_sip[3],
            ah->ar_tha[0], ah->ar_tha[1], ah->ar_tha[2], ah->ar_tha[3], ah->ar_tha[4], ah->ar_tha[5],
            ah->ar_tip[0], ah->ar_tip[1], ah->ar_tip[2], ah->ar_tip[3]);
}

static inline int create_arp_request(void *buf, unsigned char *src_mac, 
        struct sockaddr_in *saddr, struct sockaddr_in *daddr) {
    struct arppack *ap = (struct arppack *)buf; 
    ap->ar_hrd = htons(ARPHRD_ETHER);
    ap->ar_pro = htons(ETH_P_IP);
    ap->ar_hln = ETH_ALEN;
    ap->ar_pln = IPv4_LEN;
    ap->ar_op = htons(ARPOP_REQUEST);

    memcpy(ap->ar_sha, src_mac, ETH_ALEN);
    memcpy(ap->ar_sip, &saddr->sin_addr.s_addr, IPv4_LEN);

    memset(ap->ar_tha, 0, ETH_ALEN);
    memcpy(ap->ar_tip, &daddr->sin_addr.s_addr, IPv4_LEN);
    return sizeof(struct arppack);
}

#endif
