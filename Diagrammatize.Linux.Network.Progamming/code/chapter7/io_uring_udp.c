#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <liburing.h>

#define MAX_PKT_SIZE    1500
#define MAX_PKT_COUNT   10

void submit_recv(struct io_uring *ring, int sockfd, void *data)
{
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring);
    if (!sqe)
        return;

    io_uring_prep_recv(sqe, sockfd, data, MAX_PKT_SIZE, 0);
    io_uring_sqe_set_data(sqe, data);
}

void submit_all_recv(struct io_uring *ring, int sockfd)
{
    struct io_uring_sqe *sqe;
    void *data;

    while ((sqe = io_uring_get_sqe(ring))) {
        data = malloc(MAX_PKT_SIZE);
        io_uring_prep_recv(sqe, sockfd, data, MAX_PKT_SIZE, 0);
        io_uring_sqe_set_data(sqe, data);
    }
}

int do_recv(struct io_uring *ring, int sockfd)
{
    struct io_uring_cqe *cqe;
    void *data;
    int count;

    submit_all_recv(ring, sockfd);
    io_uring_submit(ring);
    count = 0;

    while (1) {
        if (io_uring_peek_cqe(ring, &cqe)) {
            io_uring_submit(ring);
            io_uring_wait_cqe(ring, &cqe);
        }
        if (!cqe) {
            fprintf(stderr, "io_uring_get_sqe failed\n");
            continue;
        }
        data = io_uring_cqe_get_data(cqe);
        count++;
        printf("recved packet count: %d, queue len:%d, res:%d, data:%s\n", count, io_uring_sq_ready(ring), cqe->res, data);
        io_uring_cqe_seen(ring, cqe);
        submit_recv(ring, sockfd, data);
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("./程序名 本机IP 服务端端口\n");
        return 0;
    }
    int ret;
    struct sockaddr_in local = {0};
    char *peer_ip = argv[1];
    unsigned short peer_port = atoi(argv[2]);

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in saddr = {0};
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = inet_addr(peer_ip); 
    saddr.sin_port = htons(peer_port);
    ret = bind(sockfd, (struct sockaddr *)&saddr, sizeof(saddr));
    if (ret < 0) {
        perror("bind");
        goto err;
    }

    struct io_uring ring;
    ret = io_uring_queue_init(10, &ring, 0);
    if (ret < 0) {
        perror("queue_init");
        goto err;
    }

    do_recv(&ring, sockfd);
err:
    close(sockfd);
    return -1;
}
