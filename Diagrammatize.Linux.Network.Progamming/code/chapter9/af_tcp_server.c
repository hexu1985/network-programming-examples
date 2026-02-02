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
    if (argc != 5) {
        printf("./a.out local_ip local_port 0或1(地址复用) 0或1(端口复用)\n");
        return 0;
    }
    char *ip = argv[1];
    unsigned short port = atoi(argv[2]);
    int on = atoi(argv[3]);
    int on1 = atoi(argv[4]);

    printf("TCP Socket option test on:%d, on1:%d\n", on, on1);
    
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }

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
    int len = 0;
    int new_sockfd;
    while(1) {
	    new_sockfd = accept(sockfd, NULL, NULL);
	    if (new_sockfd == -1) {
		    perror("accept");
		    break;
	    }

	    while(1) {
		    memset(rbuf, 0, BUF_LEN);
		    ret = recv(new_sockfd, rbuf, BUF_LEN, 0);
		    if (ret == 0) {
			    printf("recv ret:%d, errno:%d(%s)\n", ret, errno, strerror(errno));
			    break;
		    } else if ((ret == -1) && (errno != EAGAIN)) {
			    printf("recv ret:%d, errno:%d(%s)\n", ret, errno, strerror(errno));
			    break;
		    } else if ((ret == -1) && (errno == EAGAIN)) {
			    continue;
		    }
		    printf("ret:%d, rbuf:%s\n", ret, rbuf);
	    }
	    close(new_sockfd);
    }

    printf("test done!\n");
    close(sockfd);
    return 0;
}
