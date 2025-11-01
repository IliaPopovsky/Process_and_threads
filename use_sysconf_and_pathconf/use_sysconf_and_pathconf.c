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
       

printf("Обязательные параметры, которые могут быть определены с помощью функции sysconf():\n");   
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
   printf("\nATEXIT_MAX определен как %d\n", ATEXIT_MAX+0);
#else
   printf("\nидентификатор ATEXIT_MAX не найден\n");
#endif  
#ifdef _SC_ATEXIT_MAX
   pr_sysconf("ATEXIT_MAX =", _SC_ATEXIT_MAX);
#else
   printf("\nидентификатор _SC_ATEXIT_MAX не найден\n");
#endif  

#ifdef CHILD_MAX
   printf("\nCHILD_MAX определен как %d\n", CHILD_MAX+0);
#else
   printf("\nидентификатор CHILD_MAX не найден\n");
#endif  
#ifdef _SC_CHILD_MAX
   pr_sysconf("CHILD_MAX =", _SC_CHILD_MAX);
#else
   printf("\nидентификатор _SC_CHILD_MAX не найден\n");
#endif

#ifdef _SC_CLK_TCK
   pr_sysconf("\n_SC_CLK_TCK =", _SC_CLK_TCK);
#else
   printf("\nидентификатор _SC_CLK_TCK не найден\n");
#endif 

#ifdef COLL_WEIGHTS_MAX
   printf("\nCOLL_WEIGHTS_MAX определен как %d\n", COLL_WEIGHTS_MAX+0);
#else
   printf("\nидентификатор COLL_WEIGHTS_MAX не найден\n");
#endif  
#ifdef _SC_COLL_WEIGHTS_MAX
   pr_sysconf("COLL_WEIGHTS_MAX =", _SC_COLL_WEIGHTS_MAX);
#else
   printf("\nидентификатор _SC_COLL_WEIGHTS_MAX не найден\n");
#endif

#ifdef DELAYTIMER_MAX
   printf("\nDELAYTIMER_MAX определен как %d\n", DELAYTIMER_MAX+0);
#else
   printf("\nидентификатор DELAYTIMER_MAX не найден\n");
#endif  
#ifdef _SC_DELAYTIMER_MAX
   pr_sysconf("DELAYTIMER_MAX =", _SC_DELAYTIMER_MAX);
#else
   printf("\nидентификатор _SC_DELAYTIMER_MAX не найден\n");
#endif 

#ifdef HOST_NAME_MAX
   printf("\nHOST_NAME_MAX определен как %d\n", HOST_NAME_MAX+0);
#else
   printf("\nидентификатор HOST_NAME_MAX не найден\n");
#endif  
#ifdef _SC_HOST_NAME_MAX
   pr_sysconf("HOST_NAME_MAX =", _SC_HOST_NAME_MAX);
#else
   printf("\nидентификатор _SC_HOST_NAME_MAX не найден\n");
#endif 

#ifdef IOV_MAX
   printf("\nIOV_MAX определен как %d\n", IOV_MAX+0);
#else
   printf("\nидентификатор IOV_MAX не найден\n");
#endif  
#ifdef _SC_IOV_MAX
   pr_sysconf("IOV_MAX =", _SC_IOV_MAX);
#else
   printf("\nидентификатор _SC_IOV_MAX не найден\n");
#endif 

#ifdef LINE_MAX
   printf("\nLINE_MAX определен как %d\n", LINE_MAX+0);
#else
   printf("\nидентификатор LINE_MAX не найден\n");
#endif  
#ifdef _SC_LINE_MAX
   pr_sysconf("LINE_MAX =", _SC_LINE_MAX);
#else
   printf("\nидентификатор _SC_LINE_MAX не найден\n");
#endif

#ifdef LOGIN_NAME_MAX
   printf("\nLOGIN_NAME_MAX определен как %d\n", LOGIN_NAME_MAX+0);
#else
   printf("\nидентификатор LOGIN_NAME_MAX не найден\n");
#endif  
#ifdef _SC_LOGIN_NAME_MAX
   pr_sysconf("LOGIN_NAME_MAX =", _SC_LOGIN_NAME_MAX);
#else
   printf("\nидентификатор _SC_LOGIN_NAME_MAX не найден\n");
#endif

#ifdef NGROUPS_MAX
   printf("\nNGROUPS_MAX определен как %d\n", NGROUPS_MAX+0);
#else
   printf("\nидентификатор NGROUPS_MAX не найден\n");
#endif  
#ifdef _SC_NGROUPS_MAX
   pr_sysconf("NGROUPS_MAX =", _SC_NGROUPS_MAX);
#else
   printf("\nидентификатор _SC_NGROUPS_MAX не найден\n");
#endif

#ifdef OPEN_MAX
   printf("\OPEN_MAX определен как %d\n", OPEN_MAX+0);
#else
   printf("\nидентификатор OPEN_MAX не найден\n");
#endif  
#ifdef _SC_OPEN_MAX
   pr_sysconf("OPEN_MAX =", _SC_OPEN_MAX);
#else
   printf("\nидентификатор _SC_OPEN_MAX не найден\n");
#endif




printf("\nНеобязательные параметры, которые могут быть определены с помощью функции sysconf():\n");
#ifdef _POSIX_RAW_SOCKETS
   printf("_POSIX_RAW_SOCKETS определен как %ld\n", _POSIX_RAW_SOCKETS+0);
#else
   printf("идентификатор _POSIX_RAW_SOCKETS не найден\n");
#endif  
#ifdef _SC_RAW_SOCKETS
   pr_sysconf("_POSIX_RAW_SOCKETS =", _SC_RAW_SOCKETS);
#else
   printf("идентификатор _SC_RAW_SOCKETS не найден\n");
#endif 


printf("\nОбязательные параметры, которые могут быть определены с помощью функции pathconf():"); 
#ifdef MAX_CANON
   printf("\nMAX_CANON определен как %d\n", MAX_CANON+0);
#else
   printf("\nидентификатор MAX_CANON не найден\n");
#endif  
#ifdef _PC_MAX_CANON
   pr_pathconf("MAX_CANON =", argv[1], _PC_MAX_CANON);
#else
   printf("\nидентификатор _PC_MAX_CANON не найден\n");
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

static void pr_pathconf(char *mesg, char *path, int name)
{
   long val;
   
   fputs(mesg, stdout);
   errno = 0;
   if ((val = pathconf(path, name)) < 0) {
       if (errno != 0) {
           if (errno == EINVAL)
               fputs(" (не поддерживается)\n", stdout);
           else
               err_sys("ошибка вызова pathconf, path = %s", path);
       } else {
           fputs(" (не поддерживается)\n", stdout);
       }
   } else {
       printf(" %ld\n", val);
   }
}
   
   
               
