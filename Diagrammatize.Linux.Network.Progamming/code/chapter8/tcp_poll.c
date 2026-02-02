#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <poll.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define MAX_FDS 1024
#define BUF_LEN 1500

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("./a.out local_ip local_port\n");
		return 0;
	}
	char *local_ip = argv[1];
	unsigned short local_port = atoi(argv[2]);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in local = {0};
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = inet_addr(local_ip);
    local.sin_port = htons(local_port);
    int ret = bind(sockfd, (struct sockaddr *)&local, sizeof(local));
    if (ret == -1) {
        close(sockfd);
        perror("bind");
        return -1;
    }

    listen(sockfd, 5);

    struct pollfd fds_bak[MAX_FDS] = {0};
    fds_bak[0] = (struct pollfd) {.fd = sockfd, .events = POLLIN, .revents = 0}; 
    int nfds_bak = 1;
    int timeout = -1;

    while(1) {
        struct pollfd fds[MAX_FDS] = {0};
        memcpy(fds, fds_bak, sizeof(fds));
        int nfds = nfds_bak;
        ret = poll(fds, nfds, timeout); 
        if (ret == 0) {
            printf("poll timeout");
            continue;
        } else if (ret == -1){
            perror("poll");
            break;
        }

        for (int i = 0; i < nfds; i++) {
            struct pollfd *p = &fds[i];
            if (p->revents & POLLIN) {
                if (p->fd == sockfd) {
                    int new_sockfd = accept(sockfd, NULL, NULL);
                    if (new_sockfd == -1) {
                        perror("accept");
                        continue;
                    }
                    fds_bak[nfds_bak] = (struct pollfd) {.fd = new_sockfd, .events = POLLIN, .revents = 0}; 
                    nfds_bak++;
                    printf("accept nfds:%d\n", nfds_bak);
                } else {
                    char buf[BUF_LEN] = {0};
                    ret = recv(p->fd, buf, BUF_LEN, 0); 
					if ((ret == 0) || ((ret == -1) && (errno != EAGAIN))) {
                        printf("close fd:%d\n", p->fd);
                        close(p->fd);
                        fds_bak[i] = (struct pollfd) {.fd = -1, .events = 0, .revents = 0}; 

                        struct pollfd fds_tmp[MAX_FDS] = {0};
                        int pos = 0;
                        for (int j = 0; j < nfds_bak; j++) {
                            if (fds_bak[j].fd > 0) {
                                fds_tmp[pos++] = fds_bak[j];
                            }
                        }
                        memcpy(fds_bak, fds_tmp, sizeof(fds));
                        nfds_bak = pos;
                        printf("close nfds:%d\n", nfds_bak);
					} else if ((ret == -1) && (errno == EAGAIN)) {
						continue;
					} else {
						//printf("fd:%d, ret:%d, buf:%s\n", p->fd, ret, buf);
					}
                }
            }
        }
    }

    for (int j = 0; j < nfds_bak; j++) {
        if (fds_bak[j].fd > 0) {
            close(fds_bak[j].fd);
        }
    }

    return 0;
}
