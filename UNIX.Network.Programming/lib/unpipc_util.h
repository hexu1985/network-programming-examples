/* include unpipch */
/* Our own header.  Tabs are set for 4 spaces, not 8 */

#ifndef	__unpipc_util_h
#define	__unpipc_util_h

#include <sys/msg.h>
#include <sys/ipc.h>

/* Define bzero() as a macro if it's not in standard C library. */
#ifndef	HAVE_BZERO
#define	bzero(ptr,n)		memset(ptr, 0, n)
#endif

/* Posix.1g requires that an #include of <poll.h> DefinE INFTIM, but many
   systems still DefinE it in <sys/stropts.h>.  We don't want to include
   all the streams stuff if it's not needed, so we just DefinE INFTIM here.
   This is the standard value, but there's no guarantee it is -1. */
#ifndef INFTIM
#define INFTIM          (-1)    /* infinite poll timeout */
#ifdef	HAVE_POLL_H
#define	INFTIM_UNPH				/* tell unpxti.h we defined it */
#endif
#endif

/* Miscellaneous constants */
#ifndef	PATH_MAX			/* should be in <limits.h> */
#define	PATH_MAX	1024	/* max # of characters in a pathname */
#endif

#define	MAX_PATH	1024
/* $$.ix [MAX_PATH]~constant,~definition~of$$ */
#define	MAXLINE		4096	/* max text line length */
/* $$.ix [MAXLINE]~constant,~definition~of$$ */
/* $$.ix [BUFFSIZE]~constant,~definition~of$$ */
#define	BUFFSIZE	8192	/* buffer size for reads and writes */

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
					/* default permissions for new files */
/* $$.ix [FILE_MODE]~constant,~definition~of$$ */
#define	DIR_MODE	(FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH)
					/* default permissions for new directories */
/* $$.ix [DIR_MODE]~constant,~definition~of$$ */

#ifndef MSG_R 
#define MSG_R S_IRUSR 
#endif

#ifndef MSG_W 
#define MSG_W S_IWUSR 
#endif

#define	SVMSG_MODE	(MSG_R | MSG_W | MSG_R>>3 | MSG_R>>6)
//#define	SVMSG_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
					/* default permissions for new SV message queues */

#ifndef SEM_R 
#define SEM_R S_IRUSR 
#endif

#ifndef SEM_A 
#define SEM_A S_IWUSR 
#endif

/* $$.ix [SVMSG_MODE]~constant,~definition~of$$ */
#define	SVSEM_MODE	(SEM_R | SEM_A | SEM_R>>3 | SEM_R>>6)
					/* default permissions for new SV semaphores */

#ifndef SHM_R 
#define SHM_R S_IRUSR 
#endif

#ifndef SHM_W 
#define SHM_W S_IWUSR 
#endif

/* $$.ix [SVSEM_MODE]~constant,~definition~of$$ */
#define	SVSHM_MODE	(SHM_R | SHM_W | SHM_R>>3 | SHM_R>>6)
					/* default permissions for new SV shared memory */
/* $$.ix [SVSHM_MODE]~constant,~definition~of$$ */

#define	min(a,b)	((a) < (b) ? (a) : (b))
#define	max(a,b)	((a) > (b) ? (a) : (b))

typedef unsigned int uint_t;
typedef unsigned long ulong_t;

#endif	/* __unpipc_h */
