#include	"unpipc.h"
#include	"mymqueue.h"

int
main(int argc, char **argv)
{
	mymqd_t	mqd;
	void	*ptr;
	size_t	len;
	uint_t	prio;

	if (argc != 4)
		err_quit("usage: mqsend <name> <#bytes> <priority>");
	len = atoi(argv[2]);
	prio = atoi(argv[3]);

	mqd = Mymq_open(argv[1], O_WRONLY);

	ptr = Malloc(len);
	Mymq_send(mqd, ptr, len, prio);

	exit(0);
}
