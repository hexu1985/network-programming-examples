#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <netinet/tcp.h>

#define LAST_STR "good byte!"
#define BUF_LEN 1500

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("./a.out ip port\n");
        return 0;
    }
    char *ip = argv[1];
    unsigned short port = atoi(argv[2]);
    
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in local = {0};
    memset(&local, 0, sizeof(local));
    bzero(&local, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = inet_addr(ip);
    local.sin_port = htons(port);
    int ret = bind(sockfd, (struct sockaddr *)&local, sizeof(local));
    if (ret == -1) {
        close(sockfd);
        perror("bind");
        return -1;
    }

#if 1
    int qlen = 10;
    setsockopt(sockfd, SOL_TCP, TCP_FASTOPEN, &qlen, sizeof(qlen));   
#endif

    listen(sockfd, 5);

    char rbuf[BUF_LEN] = {0};
    int len = 0;
    int new_sockfd;
    while(1) {
        struct sockaddr_in peer = {0};
        socklen_t  peer_len = sizeof(peer);
	    new_sockfd = accept(sockfd, (struct sockaddr *)&peer, &peer_len);
	    if (new_sockfd == -1) {
		    perror("accept");
		    break;
	    }
        printf("new socket peer ip:port:%s:%d, addrlen:%u\n", inet_ntoa(peer.sin_addr), ntohs(peer.sin_port), peer_len);
#if 1
        struct sockaddr_in tmp1, tmp2;
        socklen_t addrlen = sizeof(struct sockaddr_in);
        getsockname(new_sockfd, (struct sockaddr *)&tmp1, &addrlen);
        printf("new socket local ip:port:%s:%d, addrlen:%u\n", inet_ntoa(tmp1.sin_addr), ntohs(tmp1.sin_port), addrlen);
        getpeername(new_sockfd, (struct sockaddr *)&tmp2, &addrlen);
        printf("new socket peer ip:port:%s:%d, addrlen:%u\n", inet_ntoa(tmp2.sin_addr), ntohs(tmp2.sin_port), addrlen);
#endif

	    while(1) {
		    memset(rbuf, 0, BUF_LEN);
		    ret = recv(new_sockfd, rbuf, BUF_LEN, 0/*MSG_PEEK*/ /*MSG_DONTWAIT*/ /*MSG_WAITALL*/);
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
#if 0
		    len = ret;
		    ret = send(new_sockfd, rbuf, len, 0); 
		    if (ret <= 0) {
			    perror("send");
			    break;
		    }
#endif
	    }
	    close(new_sockfd);
    }

#if 0
    int count = 3;
    while(count--) {
        printf("continue send\n");
        ret = send(new_sockfd, LAST_STR, strlen(LAST_STR), MSG_NOSIGNAL); 
        if (ret <= 0) {
            perror("send");
            break;
        }
        printf("send ret:%d success\n", ret);
        sleep(1);
    }
#endif
#if 0
    shutdown(new_sockfd, SHUT_WR);
    pause();
#endif

    printf("test done!\n");
    close(sockfd);
    return 0;
}
