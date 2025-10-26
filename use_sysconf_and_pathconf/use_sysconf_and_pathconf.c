// use_sysconf_and_pathconf.c
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <errno.h>		/* for definition of errno */
#include <stdarg.h>		/* ISO C variable aruments */

#define	MAXLINE	4096			/* max line length */

/*
 * Print a message and return to caller.
 * Caller specifies "errnoflag".
 */
static void err_doit(int errnoflag, int error, const char *fmt, va_list ap)
{
	char	buf[MAXLINE];

	vsnprintf(buf, MAXLINE-1, fmt, ap);
	if (errnoflag)
		snprintf(buf+strlen(buf), MAXLINE-strlen(buf)-1, ": %s",
		  strerror(error));
	strcat(buf, "\n");
	fflush(stdout);		/* in case stdout and stderr are the same */
	fputs(buf, stderr);
	fflush(NULL);		/* flushes all stdio output streams */
}

/*
 * Fatal error related to a system call.
 * Print a message and terminate.
 */
void err_sys(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(1, errno, fmt, ap);
	va_end(ap);
	exit(1);
}

/*
 * Fatal error unrelated to a system call.
 * Print a message and terminate.
 */
void
err_quit(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(0, 0, fmt, ap);
	va_end(ap);
	exit(1);
}

static void pr_sysconf(char *, int);
static void pr_pathconf(char *, char *, int);

int main(int argc, char *argv[])
{
   if (argc != 2)
       err_quit("Использование: use_sysconf_and_pathconf <каталог>");
   
#ifdef ARG_MAX
   printf("ARG_MAX определен как %d\n", ARG_MAX+0);
else
   printf("идентификатор ARG_MAX не найден\n");
#endif  
#ifdef _SC_ARG_MAX
   pr_sysconf("ARG_MAX =", _SC_ARG_MAX);
else
   printf("идентификатор _SC_ARG_MAX не найден\n");
#endif  
}
