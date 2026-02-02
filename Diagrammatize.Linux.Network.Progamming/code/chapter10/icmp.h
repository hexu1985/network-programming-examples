#ifndef __ICMP_H
#define __ICMP_H

#include <common.h>

static inline void print_icmp(const char *msg, const void *buf) {
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

static inline int create_icmp_request(void *buf, unsigned char icmp_type,
        unsigned char *data, int data_len) {
    int icmp_len = ICMP_MINLEN + data_len;
    struct icmp *icmp_pack = (struct icmp *)buf;
    icmp_pack->icmp_type = icmp_type;
    icmp_pack->icmp_code = 0;
    icmp_pack->icmp_id = 10;
    icmp_pack->icmp_seq = 20;
    memcpy(icmp_pack->icmp_data, data, data_len); 
    icmp_pack->icmp_cksum = 0;
    icmp_pack->icmp_cksum = ~checksum(icmp_pack, icmp_len);

    return icmp_len; 
}

#endif
