#include	"unp.h"

void
acpi_cli(int sockfd);

int
main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_un	servaddr;

	sockfd = Socket(AF_LOCAL, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sun_family = AF_LOCAL;
	strcpy(servaddr.sun_path, "/var/run/acpid.socket");

	Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

	acpi_cli(sockfd);		/* do it all */

	exit(0);
}

#if 0
void
acpi_cli(int sockfd)
{
	ssize_t		n;
	char		buf[MAXLINE];

again:
	while ( (n = read(sockfd, buf, MAXLINE)) > 0)
		writen(1, buf, n);

	if (n < 0 && errno == EINTR)
		goto again;
	else if (n < 0)
		err_sys("str_echo: read error");
}
#else
void
acpi_cli(int sockfd)
{
	char	recvline[MAXLINE];

    for ( ; ; ) {

		if (Readline(sockfd, recvline, MAXLINE) == 0)
			err_quit("str_cli: server terminated prematurely");

		Fputs(recvline, stdout);
	}
}
#endif
