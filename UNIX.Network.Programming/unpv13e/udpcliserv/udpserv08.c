#include	"unp.h"

int
main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in	cliaddr;

	if (argc != 3)
		err_quit("usage: udpcli02 <hostname> <service>");

	sockfd = Udp_server(argv[1], argv[2], NULL);

	dg_echo(sockfd, (SA *) &cliaddr, sizeof(cliaddr));

    exit(0);
}
