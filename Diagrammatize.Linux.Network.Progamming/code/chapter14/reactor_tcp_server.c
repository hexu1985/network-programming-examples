#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/sysinfo.h>

#include "thread_pool.h"

#define MAX_EVENTS (100000)
#define EPOLL_SIZE (100)
#define SUB_REACTOR_NUM (4)
#define POOL_SIZE (8)
#define LISTEN_BACKLOG (10)
#define BUF_LEN (4096)
#define ACK_LEN (1400)

int listen_fd = 0;
struct thread_pool g_pool = {0};
int sub_efds[SUB_REACTOR_NUM] = {0};

struct sysinfo get_memory_usage() {
    struct sysinfo info;
    if (sysinfo(&info) != 0) {
        perror("sysinfo failed");
        return info;
    }
    printf("total memory: %luKB\n", info.totalram / 1024);
    printf("free momory: %luKB\n", info.freeram / 1024);
    return info;
}

void set_nonblock(int fd) {
    int flags = fcntl(fd, F_GETFL);
    fcntl(fd, F_SETFL,  flags | O_NONBLOCK);
}

int epoll_add(int efd, int fd) {
    struct epoll_event event = {0};
    event.data.fd = fd;
    event.events = EPOLLIN;
    if (epoll_ctl(efd, EPOLL_CTL_ADD, fd, &event) == -1) {
        perror("epoll ctl ADD error");
        return -1;
    }
    return 0;
}

int epoll_del(int efd, int fd) {
    if (epoll_ctl(efd, EPOLL_CTL_DEL, fd, NULL) == -1) {
        perror("epoll ctl DEL error");
        return -1;
    }
    return 0;
}

void read_handler(void *arg) {
    int fd = (int)(intptr_t)arg;
    char buf[BUF_LEN] = {0};
    int len = recv(fd, buf, BUF_LEN, 0);
    //printf("fd:%d, len:%d, errno:%d(%s)\n", fd, len, errno, strerror(errno));
    if (len <= 0) {
        if ((errno != EAGAIN) || (len == 0)) {
            //printf("echo handler close fd:%d\n", fd);
            close(fd);
            return;
        }
    }
    //printf("fd:%d, total_len:%d\n", fd, len);
}

void write_handler(void *arg) {
    int fd = (int)(intptr_t)arg;
    char buf[BUF_LEN] = {0};
    memset(buf, 'b', BUF_LEN);
    int ret = send(fd, buf, ACK_LEN, 0);
    if (ret <= 0) {
        //printf("fd:%d, len:%d, errno:%d(%s)\n", fd, ret, errno, strerror(errno));
    }
}

int server_init(const char *ip, unsigned short port) {
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("socket error");
        return -1;
    }

    int on = 1;
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));

    struct sockaddr_in local;
    memset(&local, 0, sizeof(struct sockaddr_in));
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = inet_addr(ip);
    local.sin_port = htons(port);

    int ret = bind(sock_fd, (struct sockaddr *)&local, sizeof(struct sockaddr));
    if (ret == -1) {
        close(sock_fd);
        perror("bind error");
        return -1;
    }

    ret = listen(sock_fd, LISTEN_BACKLOG);
    if (ret == -1) {
        close(sock_fd);
        perror("listen error");
        return -1;
    }

    return sock_fd;
}

void* main_reactor(void* arg) {
    int epoll_fd = (int)(intptr_t)arg;
    printf("main reactor epoll fd:%d\n", epoll_fd);
    struct epoll_event events[MAX_EVENTS];
    int nfds;
    int timeout = 2000;
    struct sysinfo old;
    struct sysinfo new;
    old = get_memory_usage();
    int conn_count = 0;
    while (1) {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, timeout);
        if (nfds == 0) {
            new = get_memory_usage();
            unsigned int total_used = (old.freeram - new.freeram) / 1024;
            float size = (float)(total_used) / conn_count;
            printf("网络连接数量:%d, 总消耗(KB):%u, 单连接(KB):%.1f\n", conn_count, total_used, size);
        }
        for (int n = 0; n < nfds; ++n) {
            if (events[n].data.fd == listen_fd) {
                int new_fd = accept(listen_fd, NULL, NULL);
                if (new_fd == -1) {
                    perror("epoll accept error");
                    continue;
                }

                conn_count++;

                set_nonblock(new_fd);
                int sub_efd = sub_efds[new_fd % SUB_REACTOR_NUM];
                //printf("new fd:%d add epoll fd:%d\n", new_fd, sub_efd);
                int ret = epoll_add(sub_efd, new_fd);
                if (ret == -1) {
                    close(new_fd);
                }
            }
        }
    }

    close(epoll_fd);
    return NULL;
}

void* sub_reactor(void* arg) {
    int epoll_fd = (int)(intptr_t)arg;
    printf("sub reactor epoll fd:%d\n", epoll_fd);
    struct epoll_event events[MAX_EVENTS] = {0};
    int nfds;

    int timeout = 2000;
    while (1) {
        nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, timeout);
        for (int n = 0; n < nfds; ++n) {
            if (events[n].events & EPOLLIN) {
                int client_fd = events[n].data.fd;
                //printf("sub reactor epoll fd:%d, client fd:%d\n", epoll_fd, client_fd);
                read_handler((void *)(intptr_t)client_fd);
#if 1
                struct task *t = task_new(); 
                if (!t) {
                    printf("new task error\n");
                    continue;
                }
                t->handler = write_handler;
                t->arg = (void *)(intptr_t)client_fd;
                thread_pool_add(&g_pool, t);
#endif
            }
        }
    }

    close(epoll_fd);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("./程序名 本机IP 本机端口\n");
        return 0;
    }
    char *ip = argv[1];
    unsigned short port = atoi(argv[2]);
    thread_pool_init(&g_pool, POOL_SIZE);

    listen_fd = server_init(ip, port);
    printf("local ip:%s, port:%d, listen_fd:%d\n", ip, port, listen_fd);
    set_nonblock(listen_fd);

    pthread_t th[SUB_REACTOR_NUM] = {0};
    for (int i = 0; i < SUB_REACTOR_NUM; i++) {
        sub_efds[i] = epoll_create(EPOLL_SIZE);
        pthread_create(&th[i], NULL, sub_reactor, (void*)(intptr_t)sub_efds[i]);
    }

    pthread_t main_th;
    int main_efd = epoll_create(EPOLL_SIZE);
    pthread_create(&main_th, NULL, main_reactor, (void *)(intptr_t)main_efd);
    epoll_add(main_efd, listen_fd);

    pthread_join(main_th, NULL);
    for (int i = 0; i < SUB_REACTOR_NUM; i++) {
        pthread_join(th[i], NULL);
    }

    return 0;
}

