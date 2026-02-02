#include <liburing.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
 
#define PORT 8888
#define BUF_LEN 2048
#define ENTRIES 128 
 
struct io_uring ring;
struct msghdr msg_pool[ENTRIES] = {0};
struct iovec bufs[ENTRIES] = {0};
 
int create_udp_socket() {
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0) {
        return -1;
    }
 
    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT),
        .sin_addr.s_addr = INADDR_ANY
    };
 
    if (bind(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        close(sock_fd);
        return -1;
    }
    return sock_fd;
}
 
int submit_recv_request(int sock_fd, int idx, struct msghdr *msg) {
    struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);
    if (!sqe) {
        printf("get sqe error\n");
        return -1; 
    }
    io_uring_prep_recvmsg(sqe, sock_fd, msg, 0);
    io_uring_sqe_set_data64(sqe, idx);
    return 0;
}
 
int main() {
    int sock_fd = create_udp_socket();
    int ret = io_uring_queue_init(ENTRIES, &ring, 0);
    if (ret < 0) {
        return -1;
    }
    for (int i = 0; i < ENTRIES; i++) {
        bufs[i].iov_base = malloc(BUF_LEN);
        bufs[i].iov_len = BUF_LEN;
        msg_pool[i] = (struct msghdr) {
                .msg_iov = &bufs[i],
                .msg_iovlen = 1, 
        };
        submit_recv_request(sock_fd, i, &msg_pool[i]);
    }
 
    printf("UDP Server listening on port %d...\n", PORT);
 
    io_uring_submit(&ring);
    while (1) {
        struct io_uring_cqe *cqe;
        io_uring_wait_cqe(&ring, &cqe);
        int idx = (int)(intptr_t)io_uring_cqe_get_data(cqe);
        struct iovec *iov = msg_pool[idx].msg_iov;
        printf("recvfrom len:%d, idx:%d, data:%s\n", cqe->res, idx, iov->iov_base); 
        io_uring_cqe_seen(&ring, cqe);
        submit_recv_request(sock_fd, idx, &msg_pool[idx]);
        io_uring_submit(&ring);
    }
 
    for (int i = 0; i < ENTRIES; i++) {
        free(bufs[i].iov_base);
    }

    io_uring_queue_exit(&ring);
    close(sock_fd);
    return 0;
}
