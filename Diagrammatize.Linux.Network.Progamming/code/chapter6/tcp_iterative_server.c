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

#define LAST_STR "good byte!"
#define BUF_LEN 1500

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

        memset(rbuf, 0, BUF_LEN);
        ret = recv(new_sockfd, rbuf, BUF_LEN, 0);
        if (ret <= 0) {
            printf("recv ret:%d, errno:%d(%s)\n", ret, errno, strerror(errno));
            goto err;    
        }
        printf("ret:%d, rbuf:%s\n", ret, rbuf);
        ret = send(new_sockfd, LAST_STR, strlen(LAST_STR), 0);
        if (ret <= 0) {
            perror("send");
            goto err;
        }
err:    
	    close(new_sockfd);
        continue;
    }

    printf("test done!\n");
    close(sockfd);
    return 0;
}
