#include	"unp.h"

void
http_cli(int sockfd)
{
	ssize_t		n;
	char		buf[MAXLINE];
  char http_request[] = "GET / HTTP/1.1\r\n"
                        "Host: example.com\r\n"
                        "Accept: */*\r\n\r\n";

	Writen(sockfd, http_request, strlen(http_request));

	while ( (n = read(sockfd, buf, MAXLINE)) > 0) {
    Writen(STDOUT_FILENO, buf, n);
  }
}

int
main(int argc, char **argv)
{
	int					sockfd;

	sockfd = Tcp_connect("example.com", "80");

	http_cli(sockfd);		/* do it all */

	exit(0);
}
