int main(int argc, char *argv) {
    int sockfd = socket(...);
    bind(sockfd, ...);
    connect(sockfd, ...);

    while(!数据是否发送完毕) {
        int ret = send(new_sockfd, ...);
        if (ret <= 0) {
            break;
        }
    }

    shutdown(sockfd, SHUT_WR);

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

    close(sockfd);
    return 0;
}
