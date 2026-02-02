#include <netdb.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>

void dump_hostent(struct hostent *h) {
    printf("name:%s\n", h->h_name);
    char **pp = h->h_aliases;
    for (; *pp; pp++) {
        printf("aliases:%s\n", *pp);
    }
    if (h->h_addrtype == AF_INET) {
        printf("length:%d\n", h->h_length);
        pp = h->h_addr_list;
        for (; *pp; pp++) {
            char str[INET_ADDRSTRLEN];
            printf("addr:%s\n", inet_ntop(AF_INET, *pp, str, INET_ADDRSTRLEN));
        }
    } else {
        printf("not AF_INET\n");
    }
}

int gethostbyXX_test(int argc, char *argv[]) {
    if (argc != 2) { 
        printf("./程序名 域名/IP地址\n");
        return -1;
    }
#if 1
    char *name = argv[1]; //"www.baidu.com";
    struct hostent *h = gethostbyname(name);
    if (!h) {
        herror("gethostbyname"); 
        return -1;
    }
#else
    char *ip = argv[1];
    printf("ip:%s\n", ip);
    struct in_addr addr;
    if (inet_pton(AF_INET, ip, &addr) <= 0) {
        printf("inet_pton error\n");
		return -1;
	}

    struct hostent *h = gethostbyaddr(&addr, sizeof(struct in_addr), AF_INET);
    printf("h:%p\n", h);
    if (!h) {
        herror("gethostbyaddr");
        return -1;
    }
#endif
    dump_hostent(h);
    return 0;
}

void dump_servent(struct servent *ser) {
    printf("name:%s\n", ser->s_name);
    char **pp = ser->s_aliases;
    for ( ;*pp;pp++) {
        printf("aliases:%s\n", *pp);
    }
    printf("port:%d\n", ser->s_port);
    printf("proto:%s\n", ser->s_proto);
}
int getservbyXX_test(int argc, char *argv[]) {
    struct servent *ser;
#if 0
    ser = getservbyname("echo", "udp");
    if (!ser) {
        printf("getservbyname error\n", ser);
        return -1;
    }
#else
    ser = getservbyport(htons(6566), "tcp");
    if (!ser) {
        printf("getservbyport error\n", ser);
        return -1;
    }
#endif
    dump_servent(ser);
    return 0;
}

void dump_addrinfo(struct addrinfo *info) {
    printf("flags:%08x\n"
            "family:%d\n"
            "socktype:%d\n"
            "protocol:%d\n"
            "addrlen:%d\n", 
            info->ai_flags,
            info->ai_family,
            info->ai_socktype,
            info->ai_protocol,
            info->ai_addrlen);
    void *addr = NULL;
    unsigned short host_port;
    unsigned short net_port;
    if (info->ai_family == AF_INET) {
        struct sockaddr_in *ip4 = (struct sockaddr_in *)info->ai_addr;
        addr = &(ip4->sin_addr.s_addr); 
        host_port = ip4->sin_port;
        net_port = htons(ip4->sin_port);
        printf("sizeof(struct sockaddr_in)=%d\n", sizeof(struct sockaddr_in));
    } else {
        struct sockaddr_in6 *ip6 = (struct sockaddr_in6 *)info->ai_addr;
        addr = ip6->sin6_addr.s6_addr; 
        host_port = ip6->sin6_port;
        net_port = htons(ip6->sin6_port);
        printf("sizeof(struct sockaddr_in6)=%d\n", sizeof(struct sockaddr_in6));
    }

    char str[INET6_ADDRSTRLEN] = {0};
    const char *ip = inet_ntop(info->ai_family, addr, str, sizeof(str));
    printf("ip:%s, host_port/net_port:%d/%d\n", ip, host_port, net_port);
    printf("canonname:%s\n", info->ai_canonname);
}
int getaddrinfo_test(int argc, char *argv[]) {
    if (argc != 3) { 
        printf("./程序名 域名 服务名\n");
        return -1;
    }
    char *node = NULL;
    char *servname = NULL;
    int ret = strcmp(argv[1], "NULL");
    printf("ret:%d\n", ret);
    if (ret) {
        node = argv[1];
    }
    ret = strcmp(argv[2], "NULL");
    if (ret) {
        servname = argv[2];
    }
    printf("node:%s\nservname:%s\n", node, servname);

    struct addrinfo *result;
    struct addrinfo hints;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_CANONNAME;
    hints.ai_protocol = 0;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    ret = getaddrinfo(node, servname, &hints, &result);
    if (ret) {
        printf("getaddrinfo error ret:%d(%s)\n", ret, gai_strerror(ret));
        return -1;
    }
    
    struct addrinfo *r;
    for (r = result; r; r = r->ai_next) {
        dump_addrinfo(r);
    }

    return 0;
}

int main(int argc, char *argv[]) {
    //getservbyXX_test(argc, argv);
    gethostbyXX_test(argc, argv);
    //getaddrinfo_test(argc, argv);
    return 0;
}
