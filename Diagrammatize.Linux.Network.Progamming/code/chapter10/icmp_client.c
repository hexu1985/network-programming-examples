#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <linux/if_arp.h>
#include <linux/if_ether.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <arpa/inet.h>

#define IF_NAME "enp0s3"

unsigned char broadcast_mac[ETH_ALEN] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
char *dst_ip = NULL;

struct arppack {
	__be16		ar_hrd;
	__be16		ar_pro;
	unsigned char	ar_hln;
	unsigned char	ar_pln;
	__be16		ar_op;

	unsigned char		ar_sha[ETH_ALEN];
	unsigned char		ar_sip[4];
	unsigned char		ar_tha[ETH_ALEN];
	unsigned char		ar_tip[4];
};

int get_if_index(const char *if_name, int *ifindex) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct ifreq req = {0};
    memcpy(req.ifr_ifrn.ifrn_name, IF_NAME, strlen(IF_NAME));
    int ret = ioctl(sockfd, SIOCGIFINDEX, &req);
    if (ret == -1) {
        perror("ioctl");
        return -1;
    }
    *ifindex = req.ifr_ifru.ifru_ivalue;
    close(sockfd);
    return 0;
}

int get_if_mac(const char *if_name, void *mac) {
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

int get_if_ip(const char *if_name, void *addr) {
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

void print_arp(char *msg, const char *buf) {             //打印ARP数据包
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

int create_arp_request(char *buf) {
    struct ethhdr *eh = (struct ethhdr *)buf;
    memcpy(eh->h_dest, broadcast_mac, ETH_ALEN); 
    get_if_mac(IF_NAME, eh->h_source);
    eh->h_proto = htons(ETH_P_ARP);

    struct arppack *ap = (struct arppack *)(buf + ETH_HLEN); 
    ap->ar_hrd = htons(ARPHRD_ETHER);
    ap->ar_pro = htons(ETH_P_IP);
    ap->ar_hln = ETH_ALEN;
    ap->ar_pln = 4;
    ap->ar_op = htons(ARPOP_REQUEST);

    get_if_mac(IF_NAME, ap->ar_sha);
    struct sockaddr_in saddr = {0};
    get_if_ip(IF_NAME, &saddr);
    memcpy(ap->ar_sip, &saddr.sin_addr.s_addr, 4);

    memset(ap->ar_tha, 0, ETH_ALEN);
    in_addr_t dst = inet_addr(dst_ip);
    memcpy(ap->ar_tip, &dst, 4);

    return ETH_HLEN + sizeof(struct arppack);
}

#define IPHDR_LEN (20)
#define ICMP_DATA "0123456789"
#define ICMP_DATA_LEN strlen(ICMP_DATA)
#define ICMP_LEN (ICMP_MINLEN + ICMP_DATA_LEN) 

void print_iphdr(const char *msg, void *buf) {
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

void print_icmp(const char *msg, void *buf) {
    struct icmp *icmp_pack = (struct icmp *)buf;
    printf("%s\n"
            "类型(1字节):%u\n"
            "代码(1字节):%u\n"
            "校验和(2字节):0x%x\n"
            "标识(2字节):%u\n"
            "序列号(2字节):%u\n"
            "数据:%s\n",
            msg,
            icmp_pack->icmp_type,
            icmp_pack->icmp_code,
            icmp_pack->icmp_cksum,
            icmp_pack->icmp_id,
            icmp_pack->icmp_seq,
            icmp_pack->icmp_data);
}

uint16_t checksum(void *buf, int size) {
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

int create_icmp_request(char *buf, unsigned char *dst_mac, unsigned int dst_ip) {
    struct ethhdr *eh = (struct ethhdr *)buf;
    memcpy(eh->h_dest, dst_mac, ETH_ALEN); 
    get_if_mac(IF_NAME, eh->h_source);
    eh->h_proto = htons(ETH_P_IP);

    struct iphdr *iph = (struct iphdr *)(buf + ETH_HLEN); 
    iph->version = IPVERSION;
    iph->ihl = 5;
    iph->tos = 0;
    iph->tot_len = htons(IPHDR_LEN + ICMP_LEN); 
    iph->id = htons(99);
    iph->frag_off = htons(IP_DF);
    iph->ttl = MAXTTL;
    iph->protocol = IPPROTO_ICMP;
    struct sockaddr_in saddr = {0};
    get_if_ip(IF_NAME, &saddr); 
    iph->saddr = saddr.sin_addr.s_addr;
    iph->daddr = dst_ip; 
    iph->check = 0;
    iph->check = ~checksum(iph, IPHDR_LEN);

    struct icmp *icmp_pack = (struct icmp *)(buf + ETH_HLEN + IPHDR_LEN);
    icmp_pack->icmp_type = ICMP_ECHO;
    icmp_pack->icmp_code = 0;
    icmp_pack->icmp_id = 10;
    icmp_pack->icmp_seq = 20;
    memcpy(icmp_pack->icmp_data, ICMP_DATA, ICMP_DATA_LEN); 
    icmp_pack->icmp_cksum = 0;
    icmp_pack->icmp_cksum = ~checksum(icmp_pack, ICMP_LEN);

    return ETH_HLEN + IPHDR_LEN + ICMP_LEN;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("./程序名 目的IP地址\n");
        return -1;
    }

    dst_ip = argv[1];

    int sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP)); 
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }

    struct sockaddr_ll peer = {0};
    peer.sll_family = PF_PACKET;
    peer.sll_protocol = htons(ETH_P_ARP);
    get_if_index(IF_NAME, &peer.sll_ifindex);
    peer.sll_hatype = ARPHRD_ETHER;
    peer.sll_pkttype = PACKET_OTHERHOST; 
    peer.sll_halen = ETH_ALEN;

    char buf[ETH_FRAME_LEN] = {0};
    char rbuf[ETH_FRAME_LEN] = {0};
    int len = create_arp_request(buf);
    int ret = 0;
    struct arppack ack_pack = {0};
    while(1) {
        print_arp("ARP请求-----------------", buf+ETH_HLEN);
        ret = sendto(sockfd, buf, len, 0, (struct sockaddr *)&peer, sizeof(peer));
        if (ret <= 0) {
            perror("sendto");
            break;
        }

        memset(rbuf, 0, ETH_FRAME_LEN);
        ret = recvfrom(sockfd, rbuf, ETH_FRAME_LEN, 0, NULL, NULL);
        if (ret <= 0) {
            perror("recvfrom");
            break;
        }
        
        memcpy(&ack_pack, rbuf + ETH_HLEN, sizeof(ack_pack));
        print_arp("ARP响应-----------------", rbuf+ETH_HLEN);
        break;
    }

    close(sockfd);

    /*******************************************************/
    int icmp_sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_IP));
    if (icmp_sockfd == -1) {
        perror("socket");
        return -1;
    }

    struct sockaddr_ll icmp_peer = {0};
    icmp_peer.sll_family = PF_PACKET;
    icmp_peer.sll_protocol = htons(ETH_P_IP);
    get_if_index(IF_NAME, &icmp_peer.sll_ifindex);
    icmp_peer.sll_hatype = ARPHRD_ETHER;
    icmp_peer.sll_pkttype = PACKET_OTHERHOST; 
    icmp_peer.sll_halen = ETH_ALEN;
    memcpy(icmp_peer.sll_addr, ack_pack.ar_sha, ETH_ALEN);

    char icmp_sbuf[ETH_FRAME_LEN] = {0};
    char icmp_rbuf[ETH_FRAME_LEN] = {0};
    int icmp_len = 0;
    while(1) {
        memset(icmp_sbuf, 0, ETH_FRAME_LEN);
        icmp_len = create_icmp_request(icmp_sbuf, ack_pack.ar_sha, *(unsigned int *)ack_pack.ar_sip);
        printf("icmp_len:%d\n", icmp_len);
        print_iphdr("发送IP数据包-----------------", icmp_sbuf + ETH_HLEN);
        print_icmp("", icmp_sbuf + ETH_HLEN + IPHDR_LEN);
        ret = sendto(icmp_sockfd, icmp_sbuf, icmp_len, 0, (struct sockaddr *)&icmp_peer, sizeof(icmp_peer));
        if (ret <= 0) {
            perror("sendto");
            break;
        }

        while (1) {
            memset(icmp_rbuf, 0, ETH_FRAME_LEN);
            ret = recvfrom(icmp_sockfd, icmp_rbuf, ETH_FRAME_LEN, 0, NULL, NULL);
            if (ret <= 0) {
                perror("recvfrom");
                break;
            }

            struct iphdr *iph = (struct iphdr *)(icmp_rbuf + ETH_HLEN);
            struct icmp *ack_icmp = (struct icmp *)(icmp_rbuf + ETH_HLEN + IPHDR_LEN);
            if ((iph->protocol == IPPROTO_ICMP) && (iph->daddr == *(unsigned int *)ack_pack.ar_tip)) {
                print_iphdr("接收IP数据包++++++++++++++++", iph);
                uint16_t ip_csum = checksum(iph, IPHDR_LEN);
                uint16_t icmp_csum = checksum(ack_icmp, IPHDR_LEN);
                printf("ip csum:0x%04x, icmp csum:0x%04x\n", ip_csum, icmp_csum);
                print_icmp("", ack_icmp);
                break; 
            }
        }

        sleep(3);
    }

    close(icmp_sockfd);

    return 0;
}
