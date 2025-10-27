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
#else
   printf("идентификатор ARG_MAX не найден\n");
#endif  
#ifdef _SC_ARG_MAX
   pr_sysconf("ARG_MAX =", _SC_ARG_MAX);
#else
   printf("идентификатор _SC_ARG_MAX не найден\n");
#endif 

#ifdef ATEXIT_MAX
   printf("ATEXIT_MAX определен как %d\n", ATEXIT_MAX+0);
#else
   printf("идентификатор ATEXIT_MAX не найден\n");
#endif  
#ifdef _SC_ATEXIT_MAX
   pr_sysconf("ATEXIT_MAX =", _SC_ATEXIT_MAX);
#else
   printf("идентификатор _SC_ATEXIT_MAX не найден\n");
#endif  

#ifdef CHILD_MAX
   printf("CHILD_MAX определен как %d\n", CHILD_MAX+0);
#else
   printf("идентификатор CHILD_MAX не найден\n");
#endif  
#ifdef _SC_CHILD_MAX
   pr_sysconf("CHILD_MAX =", _SC_CHILD_MAX);
#else
   printf("идентификатор _SC_CHILD_MAX не найден\n");
#endif

#ifdef _SC_CLK_TCK
   pr_sysconf("_SC_CLK_TCK =", _SC_CLK_TCK);
#else
   printf("идентификатор _SC_CLK_TCK не найден\n");
#endif 

#ifdef COLL_WEIGHTS_MAX
   printf("COLL_WEIGHTS_MAX определен как %d\n", COLL_WEIGHTS_MAX+0);
#else
   printf("идентификатор COLL_WEIGHTS_MAX не найден\n");
#endif  
#ifdef _SC_COLL_WEIGHTS_MAX
   pr_sysconf("COLL_WEIGHTS_MAX =", _SC_COLL_WEIGHTS_MAX);
#else
   printf("идентификатор _SC_COLL_WEIGHTS_MAX не найден\n");
#endif

#ifdef DELAYTIMER_MAX
   printf("DELAYTIMER_MAX определен как %d\n", DELAYTIMER_MAX+0);
#else
   printf("идентификатор DELAYTIMER_MAX не найден\n");
#endif  
#ifdef _SC_DELAYTIMER_MAX
   pr_sysconf("DELAYTIMER_MAX =", _SC_DELAYTIMER_MAX);
#else
   printf("идентификатор _SC_DELAYTIMER_MAX не найден\n");
#endif 

#ifdef HOST_NAME_MAX
   printf("HOST_NAME_MAX определен как %d\n", HOST_NAME_MAX+0);
#else
   printf("идентификатор HOST_NAME_MAX не найден\n");
#endif  
#ifdef _SC_HOST_NAME_MAX
   pr_sysconf("HOST_NAME_MAX =", _SC_HOST_NAME_MAX);
#else
   printf("идентификатор _SC_HOST_NAME_MAX не найден\n");
#endif 

   exit(0);

}

static void pr_sysconf(char *mesg, int name)
{
   long val;
   
   fputs(mesg, stdout);
   errno = 0;
   if ((val = sysconf(name)) < 0) {
       if (errno != 0) {
           if (errno == EINVAL)
               fputs(" (не поддерживается)\n", stdout);
           else
               err_sys("ошибка вызова sysconf");
       } else {
           fputs(" (нет ограничений)\n", stdout);
       }
   } else {
       printf(" %ld\n", val);
   }
} 
   
   
               
