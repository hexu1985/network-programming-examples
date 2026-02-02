int main(int argc, char *argv[]) {
    int sockfd = socket(...);
    bind(sockfd, ...);
    listen(sockfd, 5);
    int new_sockfd = accept(sockfd, ...);
    while(1) {
        int ret = recv(new_sockfd, ...);
        if (ret == 0) {
            break;
        } else if ((ret == -1) && (errno != EAGAIN)) {
            break;
        } else if ((ret == -1) && (errno == EAGAIN)) {
            continue;
        }
    }


    while(!剩余数据是否发送完毕) {
        int ret = send(new_sockfd, ...);
        if (ret <= 0) {
            break;
        }
    }

    close(new_sockfd);
    close(sockfd);

    return 0;
}
