#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <netinet/tcp.h>
#include <pthread.h>

#define LAST_STR "good byte!"
#define BUF_LEN 1500

void *client_proc(void *arg) {
    int sockfd = (int)(long)arg;
    char rbuf[BUF_LEN] = {0};
    int ret;
    while(1) {
        memset(rbuf, 0, BUF_LEN);
        ret = recv(sockfd, rbuf, BUF_LEN, 0);
        if (ret <= 0) {
            printf("recv ret:%d, errno:%d(%s)\n", ret, errno, strerror(errno));
            break;
        }
        printf("sockfd:%d, ret:%d, rbuf:%s\n",sockfd, ret, rbuf);
        ret = send(sockfd, LAST_STR, strlen(LAST_STR), 0);
        if (ret <= 0) {
            perror("send");
            break;
        }
    }
    close(sockfd);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("./a.out local_ip local_port\n");
        return 0;
    }
    char *ip = argv[1];
    unsigned short port = atoi(argv[2]);
    
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }

    int on = 1; 
    int on1 = 1; 
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &on1, sizeof(on1));

    struct sockaddr_in local = {0};
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = inet_addr(ip);
    local.sin_port = htons(port);
    int ret = bind(sockfd, (struct sockaddr *)&local, sizeof(local));
    if (ret == -1) {
        close(sockfd);
        perror("bind");
        return -1;
    }

    listen(sockfd, 10);

    char rbuf[BUF_LEN] = {0};
    int new_sockfd;
    while(1) {
	    new_sockfd = accept(sockfd, NULL, NULL);
	    if (new_sockfd == -1) {
		    perror("accept");
		    break;
	    }
        pthread_t th;
        int ret = pthread_create(&th, NULL, client_proc, (void*)(long)new_sockfd);
        if (ret != 0) {
            close(new_sockfd);
            continue;
        }
        pthread_detach(th);
    }

    printf("test done!\n");
    close(sockfd);
    return 0;
}
