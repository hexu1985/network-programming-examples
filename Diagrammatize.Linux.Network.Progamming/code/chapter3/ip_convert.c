#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
#if 0
    char *p = argv[1];
    in_addr_t ip = inet_addr(p); 
    in_addr_t ip1= inet_network(p);
    printf("ret:%d, ret1:%d\n", ip, ip1);
    struct in_addr addr;
    inet_aton(p, &addr);
    struct in_addr addr1;
    inet_pton(AF_INET, p, &addr1);
    printf("ip:%08x, ip1:%08x, addr n/h:%08x/%08x, addr1:%08x\n", ip, ip1, addr.s_addr, ntohl(addr.s_addr), addr1.s_addr);
#elif 0
    //in_addr_t ip = inet_addr(p);
    struct in_addr addr = {.s_addr = 0xC0A80101};
    in_addr_t net_id = inet_netof(addr);
    in_addr_t host_id = inet_lnaof(addr);
    printf("host_id:%08x, net_id:%08x\n", host_id, net_id);
#elif 0
    in_addr_t net_id = 0xC0A80C;
    in_addr_t host_id = 0x22;
    struct in_addr addr = inet_makeaddr(net_id, host_id);
    printf("net ip:%08X，char* ip:%s\n", addr.s_addr, inet_ntoa(addr));
#else
    struct in_addr addr = {.s_addr = 0x0101A8C0};
#if 1
    char *ip = inet_ntoa(addr);
    printf("ip:%s\n", ip);
    addr.s_addr = 0x0202A8C0;
    char *ip1 = inet_ntoa(addr);
    printf("ip:%s,ip1:%s\n", ip, ip1);
#else
    char str[INET_ADDRSTRLEN] = {0};
    const char *ip = inet_ntop(AF_INET, &addr, str, sizeof(str));
    printf("ip:%s, str:%s\n", ip, str);
#endif
#endif
    return 0;
}
