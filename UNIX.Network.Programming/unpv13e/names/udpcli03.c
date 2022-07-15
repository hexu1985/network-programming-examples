#include	"unp.h"

int
main(int argc, char **argv)
{
	int					sockfd;

	if (argc != 3)
		err_quit("usage: daytimetcpcli <hostname/IPaddress> <service/port#>");

	sockfd = Udp_connect(argv[1], argv[2]);

	dg_cli(stdin, sockfd, NULL, 0);

	exit(0);
}
