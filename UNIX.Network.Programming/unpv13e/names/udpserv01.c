#include	"unp.h"

int
main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in	cliaddr;

	if (argc == 2)
		sockfd = Udp_server(NULL, argv[1], NULL);
	else if (argc == 3)
		sockfd = Udp_server(argv[1], argv[2], NULL);
	else
		err_quit("usage: daytimetcpsrv4 [ <host> ] <service or port>");

	dg_echo(sockfd, (SA *) &cliaddr, sizeof(cliaddr));
}
