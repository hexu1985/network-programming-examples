#include "common.h"
#include "ether.h"
#include "ip.h"
#include "arp.h"
#include "icmp.h"
#include "udp.h"

#define PEER_MAC {0x14, 0x23, 0x0a, 0x39, 0xe3, 0x75}
#define GW_IP   "192.168.1.1"
char *dst_ip;

int arp_test(void *buf, const char *if_name, const char *dst_ip) {
    unsigned char src_mac[ETH_ALEN] = {0};
    unsigned char dst_mac[ETH_ALEN] = {0};
    get_if_mac(if_name, src_mac);
    memset(dst_mac, 0xff, sizeof(dst_mac));
    int len1 = create_ethhdr(buf, src_mac, dst_mac, ETH_P_ARP);
    print_ethhdr("ARP ethhdr--------------", buf);
    struct sockaddr_in saddr;
    struct sockaddr_in daddr;
    get_if_ip(if_name, &saddr);
    daddr.sin_addr.s_addr = inet_addr(dst_ip);
    int len2 = create_arp_request((char *)buf + len1, src_mac, &saddr, &daddr);
    print_arp("ARP请求-----------------", buf + len1);
    printf("\n\n");
    return len1 + len2;
}

int icmp_test(void *buf, const char *if_name, const char *dst_ip, char *data) {
    unsigned char src_mac[ETH_ALEN] = {0};
    unsigned char dst_mac[ETH_ALEN] = PEER_MAC;
    get_if_mac(if_name, src_mac);
    int len1 = create_ethhdr(buf, src_mac, dst_mac, ETH_P_IP);

    print_ethhdr("ICMP ethhdr-------------", buf);
    struct sockaddr_in saddr;
    struct sockaddr_in daddr;
    get_if_ip(if_name, &saddr);
    daddr.sin_addr.s_addr = inet_addr(dst_ip);
    int data_len = strlen(data);
    int icmp_len = ICMP_MINLEN + data_len; 
    int len2 = create_iphdr(buf + len1, &saddr, &daddr, IPPROTO_ICMP, icmp_len);
    print_iphdr("ICMP iphdr--------------", buf + len1);

    int len3 = create_icmp_request(buf + len1 + len2, ICMP_ECHO, (unsigned char *)data, data_len);
    print_icmp("ICMP ECHO-----------------", buf + len1 + len2);
    printf("\n\n");

    return len1 + len2 + len3;
}

int udp_test(void *buf, const char *if_name, const char *dst_ip, unsigned short sport, unsigned short dport, char *data) {
    unsigned char src_mac[ETH_ALEN] = {0};
    unsigned char dst_mac[ETH_ALEN] = PEER_MAC;
    get_if_mac(if_name, src_mac);
    get_dst_mac(if_name, GW_IP, dst_mac);
    int len1 = create_ethhdr(buf, src_mac, dst_mac, ETH_P_IP);

    print_ethhdr("UDP ethhdr-------------", buf);
    struct sockaddr_in saddr;
    struct sockaddr_in daddr;
    get_if_ip(if_name, &saddr);
    daddr.sin_addr.s_addr = inet_addr(dst_ip);
    int data_len = strlen(data);
    int udp_len = UDPHDR_LEN + data_len; 
    int len2 = create_iphdr(buf + len1, &saddr, &daddr, IPPROTO_UDP, udp_len);
    print_iphdr("UDP iphdr--------------", buf + len1);

    int len3 = create_udp_pack(buf + len1 + len2, sport, dport, &saddr, &daddr, data, data_len); 
    print_udp("UDP request-----------------", buf + len1 + len2);
    printf("\n\n");

    return len1 + len2 + len3;
}

void usage() {
    printf("arp测试命令：./程序名  arp 网卡名 目的IP\n");
    printf("arp测试命令：./程序名 icmp 网卡名 目的IP \n");
    printf("arp测试命令：./程序名  udp 网卡名 目的IP 源端口 目的端口\n");
}

void *recv_proc(void *arg) {
    int sockfd = (int)arg;
    printf("recv proc sockfd:%d\n", sockfd);
    char rbuf[ETH_FRAME_LEN] = {0};
    int ret;
    while(1) {
        memset(rbuf, 0, ETH_FRAME_LEN);
        struct sockaddr_ll tmp = {0};
        socklen_t addrlen = sizeof(tmp);
        ret = recvfrom(sockfd, rbuf, ETH_FRAME_LEN, 0, (struct sockaddr *)&tmp, &addrlen);
        if (ret == 0) {
            printf("recvfrom ret:%d, errno:%d(%s)\n", ret, errno, strerror(errno));
            break;
        } else if ((ret == -1) && (errno != EAGAIN)) {
            printf("recvfrom ret:%d, errno:%d(%s)\n", ret, errno, strerror(errno));
            break;
        } else if ((ret == -1) && (errno == EAGAIN)) {
            continue;
        }
        //print_sockaddr_ll("struct sockaddr_ll地址+++++++++++++++++++", &tmp);

        struct ethhdr *eh = (struct ethhdr *)rbuf;
        if (eh->h_proto == htons(ETH_P_ARP)) {
            print_arp("ARP+++++++++++++++++", rbuf + ETH_HLEN);
        } else if (eh->h_proto == htons(ETH_P_IP)) {
            struct iphdr *iph = (struct iphdr *)(rbuf + ETH_HLEN);
            if (iph->protocol == IPPROTO_TCP) continue; 
            printf("recv ip:%s, protocol:%d\n", 
                    inet_ntoa((struct in_addr){.s_addr = iph->saddr}),
                    iph->protocol);
            struct sockaddr_in daddr;
            daddr.sin_addr.s_addr = inet_addr(dst_ip);
            bool isvalid = 1;//(iph->saddr == daddr.sin_addr.s_addr);
            if (isvalid) {
                if (iph->protocol == IPPROTO_ICMP) {
                    print_iphdr("ICMP iphdr+++++++++++++", iph);
                    print_icmp("ICMP应答++++++++++++++++", rbuf + ETH_HLEN + IPHDR_LEN);
                } else if (iph->protocol == IPPROTO_UDP) {
                    print_iphdr("UDP iphdr+++++++++++++", iph);
                    print_udp("UDP应答+++++++++++++++++", rbuf + ETH_HLEN + IPHDR_LEN);
                }
                printf("\n\n");
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        usage();
        return -1;    
    }
    int ret;
    char *if_name;
    unsigned short proto; 
    unsigned short sport;
    unsigned short dport;
    int sockfd = -1;
    int udp_sockfd = -1;

    printf("argv[1]:%s\n", argv[1]);
    if (!strcmp(argv[1], "arp")) {
        printf("start arp test\n");
        if_name = argv[2];
        dst_ip = argv[3];
        proto = ETH_P_ARP;
    } else if (!strcmp(argv[1], "icmp")) {
        printf("start icmp test\n");
        if_name = argv[2];
        dst_ip = argv[3];
        proto = ETH_P_IP;
    } else if (!strcmp(argv[1], "udp")) {
        printf("start udp test\n");
        if_name = argv[2];
        dst_ip = argv[3];
        sport = atoi(argv[4]);
        dport = atoi(argv[5]);
        proto = ETH_P_IP;

        /* 防止接收数据时，找不到对应端口的UDP套接字（原始套接字不是UDP套接字），发送错误ICMP包至对端 */
        udp_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (udp_sockfd == -1) {
            perror("socket");
            goto out;
        }

        struct sockaddr_in local = {0};
        local.sin_family = AF_INET;
        get_if_ip(if_name, &local);
        local.sin_port = htons(sport);
        int ret = bind(udp_sockfd, (struct sockaddr *)&local, sizeof(local));
        if (ret == -1) {
            perror("bind");
            goto out;
        }
    } else {
        usage();
        return -1;
    }

    //proto = ETH_P_ALL;
    sockfd = socket(AF_PACKET, SOCK_RAW, htons(proto)); 
    if (sockfd == -1) {
        perror("socket");
        goto out;
    }

#if 0
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, if_name, IFNAMSIZ);
    if (ioctl(sockfd, SIOCGIFFLAGS, &ifr) == -1) {
        perror("ioctl SIOCGIFFLAGS");
        goto out;
    }
    //ifr.ifr_flags |= IFF_PROMISC;
    ifr.ifr_flags &= ~IFF_PROMISC;
    if (ioctl(sockfd, SIOCSIFFLAGS, &ifr) == -1) {
        perror("ioctl SIOCSIFFLAGS");
        goto out;
    }
#endif

#if 0
    struct sockaddr_ll local = {0};
    local.sll_family = AF_PACKET;
    local.sll_protocol = htons(proto);
    get_if_index(if_name, &local.sll_ifindex);
    ret = bind(sockfd, (struct sockaddr *)&local, sizeof(local));
    if (ret == -1) {
        perror("bind");
        goto out;
    }
#endif

#if 1
    struct sockaddr_ll peer = {0};
    get_if_index(if_name, &peer.sll_ifindex);
#else
    struct sockaddr_ll peer = {0};
    peer.sll_family = AF_PACKET;
    peer.sll_protocol = htons(proto);
    get_if_index(if_name, &peer.sll_ifindex);
    peer.sll_hatype = ARPHRD_ETHER;
    peer.sll_pkttype = PACKET_OTHERHOST; 
    peer.sll_halen = ETH_ALEN;
#endif

    char sbuf[ETH_FRAME_LEN] = {0};
    int len;
    char *data = "helloworld";

    pthread_t th;
    pthread_create(&th, NULL, recv_proc, (void *)sockfd);

    while(1) {
        memset(sbuf, 0, sizeof(sbuf));
        if (!strcmp(argv[1], "arp")) {
            len = arp_test(sbuf, if_name, dst_ip);
        } else if (!strcmp(argv[1], "icmp")) {
            len = icmp_test(sbuf, if_name, dst_ip, data);
        } else if (!strcmp(argv[1], "udp")) {
            len = udp_test(sbuf, if_name, dst_ip, sport, dport, data);
        }
        printf("send buf len:%d\n", len);
        ret = sendto(sockfd, sbuf, len, 0, (struct sockaddr *)&peer, sizeof(peer));
        //ret = sendto(sockfd, sbuf, len, 0, NULL, 0);
        if (ret <= 0) {
            printf("sendto errno:%d(%s)\n", errno, strerror(errno));
            break;
        }

        sleep(1);
    }

out:
    if (udp_sockfd > 1) 
        close(udp_sockfd);
    if (sockfd > 1)
        close(sockfd);

    return 0;
}
