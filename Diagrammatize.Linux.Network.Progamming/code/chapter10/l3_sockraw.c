#include "common.h"
#include "ip.h"
#include "icmp.h"
#include "udp.h"

struct sockaddr_in peer = {0};

int iphdr_icmp_test(void *buf, const char *if_name, const char *dst_ip, char *data) {
    struct sockaddr_in saddr;
    struct sockaddr_in daddr;
    get_if_ip(if_name, &saddr);
    daddr.sin_addr.s_addr = inet_addr(dst_ip);
    int data_len = strlen(data);
    int icmp_len = ICMP_MINLEN + data_len; 
    int len1 = create_iphdr(buf, &saddr, &daddr, IPPROTO_ICMP, icmp_len);
    print_iphdr("ICMP iphdr--------------", buf);
    int len2 = create_icmp_request(buf + len1, ICMP_ECHO, (unsigned char *)data, data_len);
    print_icmp("ICMP ECHO-----------------", buf + len1);
    printf("\n\n");
    return len1 + len2;
}
int icmp_test(void *buf, char *data) {
    int data_len = strlen(data);
    int len1 = create_icmp_request(buf, ICMP_ECHO, (unsigned char *)data, data_len);
    print_icmp("ICMP ECHO-----------------", buf);
    printf("\n\n");
    return len1;
}

int iphdr_udp_test(void *buf, const char *if_name, const char *dst_ip, unsigned short sport, unsigned short dport, char *data) {
    struct sockaddr_in saddr;
    struct sockaddr_in daddr;
    get_if_ip(if_name, &saddr);
    daddr.sin_addr.s_addr = inet_addr(dst_ip);
    int data_len = strlen(data);
    int udp_len = UDPHDR_LEN + data_len; 
    int len1 = create_iphdr(buf, &saddr, &daddr, IPPROTO_UDP, udp_len);
    print_iphdr("UDP iphdr--------------", buf);
    int len2 = create_udp_pack(buf + len1, sport, dport, &saddr, &daddr, data, data_len); 
    print_udp("UDP request-----------------", buf + len1);
    printf("\n\n");
    return len1 + len2;
}

int udp_test(void *buf, const char *if_name, const char *dst_ip, unsigned short sport, unsigned short dport, char *data) {
    struct sockaddr_in saddr;
    struct sockaddr_in daddr;
    get_if_ip(if_name, &saddr);
    daddr.sin_addr.s_addr = inet_addr(dst_ip);
    int data_len = strlen(data);
    int len1 = create_udp_pack(buf, sport, dport, &saddr, &daddr, data, data_len); 
    print_udp("UDP request-----------------", buf);
    printf("\n\n");
    return len1;
}

void usage() {
    printf("arp测试命令：./程序名 icmp 网卡名 目的IP \n");
    printf("arp测试命令：./程序名  udp 网卡名 目的IP 源端口 目的端口\n");
}

void *recv_proc(void *arg) {
    int sockfd = (int)arg;
    printf("recv proc sockfd:%d\n", sockfd);
    char rbuf[ETH_FRAME_LEN] = {0};
    while (1) {
        memset(rbuf, 0, ETH_FRAME_LEN);
        struct sockaddr_in tmp;
        socklen_t addrlen = sizeof(struct sockaddr_in);
        int ret = recvfrom(sockfd, rbuf, ETH_FRAME_LEN, 0, (struct sockaddr *)&tmp, &addrlen);
        if (ret == 0) {
            printf("recvfrom ret:%d, errno:%d(%s)\n", ret, errno, strerror(errno));
            break;
        } else if ((ret == -1) && (errno != EAGAIN)) {
            printf("recvfrom ret:%d, errno:%d(%s)\n", ret, errno, strerror(errno));
            break;
        } else if ((ret == -1) && (errno == EAGAIN)) {
            continue;
        }
        printf("tmp ip:port:%s:%d, addrlen:%u\n", inet_ntoa(tmp.sin_addr), ntohs(tmp.sin_port), addrlen);

        struct iphdr *iph = (struct iphdr *)(rbuf);
        printf("recv ip:%s\n", inet_ntoa((struct in_addr){.s_addr = iph->saddr}));
        print_iphdr("iphdr++++++++++++++++++", iph); 
        if (iph->saddr == peer.sin_addr.s_addr) {
            if (iph->protocol == IPPROTO_ICMP) {
                print_iphdr("应答包iphdr++++++++++++++++++", iph); 
                print_icmp("ICMP应答++++++++++++++++", rbuf + IPHDR_LEN);
                printf("\n\n");
            } else if (iph->protocol == IPPROTO_UDP) {
                print_iphdr("应答包iphdr++++++++++++++++++", iph); 
                struct udphdr *udph = (struct udphdr *)(rbuf + IPHDR_LEN);
#if 1
                uint16_t ip_csum = checksum(iph, IPHDR_LEN);
                if (ip_csum == 0xFFFF) {
                    printf("IP checksum success\n");
                }
                char tmp[2048] = {0};
                struct pseudo_udphdr *pudph = (struct pseudo_udphdr *)tmp;
                pudph->saddr = iph->saddr;
                pudph->daddr = iph->daddr;
                pudph->fill = 0;
                pudph->protocol = IPPROTO_UDP;
                pudph->len = udph->uh_ulen;
                int ulen = ntohs(udph->uh_ulen);
                memcpy(tmp + sizeof(struct pseudo_udphdr), udph, ulen);
                ulen = (ulen % 2) ? ulen + 1 : ulen;
                uint16_t udp_csum = checksum(tmp, sizeof(struct pseudo_udphdr) + ulen);
                if (udp_csum == 0xFFFF) {
                    printf("UDP checksum success\n");
                }
#endif
                print_udp("UDP应答++++++++++++++", udph);
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
    char *if_name;
    char *dst_ip;
    unsigned short sport;
    unsigned short dport;
    int protocol;
    printf("argv[1]:%s\n", argv[1]);
    int udp_sockfd = -1;
    int sockfd = -1;
    if (!strcmp(argv[1], "icmp")) {
        printf("start icmp test\n");
        if_name = argv[2];
        dst_ip = argv[3];
        protocol = IPPROTO_ICMP;
    } else if (!strcmp(argv[1], "udp")) {
        printf("start udp test\n");
        if_name = argv[2];
        dst_ip = argv[3];
        sport = atoi(argv[4]);
        dport = atoi(argv[5]);
        protocol = IPPROTO_UDP;

#if 1
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
#endif
    } else {
        usage();
        return -1;
    }

    //protocol = IPPROTO_RAW;
    //protocol = IPPROTO_IP;
    sockfd = socket(AF_INET, SOCK_RAW, protocol);
    if (sockfd == -1) {
        printf("socket errno:%d(%s)\n", errno, strerror(errno));
        goto out;
    }

#if 0
    struct sockaddr_in local = {0};
    local.sin_family = AF_INET;
    get_if_ip(if_name, &local);
    //local.sin_port = htons(1111);
    int ret = bind(sockfd, (struct sockaddr *)&local, sizeof(local));
    if (ret == -1) {
        perror("bind");
        goto out;
    }
#endif

    int on = 1;
    setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on));

    peer.sin_family = AF_INET;
    peer.sin_addr.s_addr = inet_addr(dst_ip);

    struct sockaddr_in peer1;
    peer1.sin_family = AF_INET;
    peer1.sin_addr.s_addr = inet_addr("192.168.2.200");

#if 0
    ret = connect(sockfd, (struct sockaddr *)&peer, sizeof(peer));
    if (ret == -1) {
        perror("connect");
        goto out;
    }
#endif

    char sbuf[ETH_FRAME_LEN] = {0};
    int len = 0;
    char *data = "helloworld";

    pthread_t th;
    pthread_create(&th, NULL, recv_proc, (void *)sockfd);

    while(1) {
        memset(sbuf, 0, ETH_FRAME_LEN);
        if (on == 1) {
            if (!strcmp(argv[1], "icmp")) {
                printf("iphdr_icmp_test\n");
                len = iphdr_icmp_test(sbuf, if_name, dst_ip, data);
            } else if (!strcmp(argv[1], "udp")) {
                printf("iphdr_udp_test\n");
                len = iphdr_udp_test(sbuf, if_name, dst_ip, sport, dport, data);
            }
        } else {
            if (!strcmp(argv[1], "icmp")) {
                printf("icmp_test\n");
                len = icmp_test(sbuf, data);
            } else if (!strcmp(argv[1], "udp")) {
                printf("udp_test\n");
                len = udp_test(sbuf, if_name, dst_ip, sport, dport, data);
            }
        }
        printf("send len:%d***\n", len);
        int ret = sendto(sockfd, sbuf, len, 0, (struct sockaddr *)&peer, sizeof(peer));
        //int ret = sendto(sockfd, sbuf, len, 0, NULL, 0);
        //int ret = send(sockfd, sbuf, len, 0);
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
