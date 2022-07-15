#include	"unp.h"

void	pr_ipv4(char **);

int
main(int argc, char **argv)
{
	int				n;
	char			*ptr;
	struct addrinfo	hints, *res, *ressave;

	while (--argc > 0) {
        bzero(&hints, sizeof(struct addrinfo));
        hints.ai_flags = AI_CANONNAME;
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

		ptr = *++argv;
        if ( (n = getaddrinfo(ptr, NULL, &hints, &res)) != 0) {
            err_msg("getaddrinfo error for host %s: %s", 
                    ptr, gai_strerror(n));
			continue;
        }
		printf("official host name: %s\n", res->ai_canonname);

        ressave = res;
        do {
            printf("\taddress: %s\n",
                    Sock_ntop(res->ai_addr, res->ai_addrlen));
        } while ( (res = res->ai_next) != NULL);

        freeaddrinfo(ressave);
	}
	exit(0);
}

