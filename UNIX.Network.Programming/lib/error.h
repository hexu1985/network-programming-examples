#ifndef	__unp_error_h
#define __unp_error_h

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#define	MAXLINE		4096	/* max text line length */

/* Define to 1 if you have the `vsnprintf' function. */
#define HAVE_VSNPRINTF 1

void	 err_dump(const char *, ...);
void	 err_msg(const char *, ...);
void	 err_quit(const char *, ...);
void	 err_ret(const char *, ...);
void	 err_sys(const char *, ...);

#endif

