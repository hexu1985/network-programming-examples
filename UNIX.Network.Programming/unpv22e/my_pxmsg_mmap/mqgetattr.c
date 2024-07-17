#include	"unpipc.h"
#include	"mymqueue.h"

int
main(int argc, char **argv)
{
	mymqd_t	mqd;
	struct mymq_attr	attr;

	if (argc != 2)
		err_quit("usage: mqgetattr <name>");

	mqd = Mymq_open(argv[1], O_RDONLY);

	Mymq_getattr(mqd, &attr);
	printf("max #msgs = %ld, max #bytes/msg = %ld, "
		   "#currently on queue = %ld\n",
		   attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);

	Mymq_close(mqd);
	exit(0);
}
