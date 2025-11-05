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
   printf("\nOPEN_MAX определен как %d\n", OPEN_MAX+0);
#else
   printf("\nидентификатор OPEN_MAX не найден\n");
#endif  
#ifdef _SC_OPEN_MAX
   pr_sysconf("OPEN_MAX =", _SC_OPEN_MAX);
#else
   printf("\nидентификатор _SC_OPEN_MAX не найден\n");
#endif

#ifdef PAGESIZE
   printf("\nPAGESIZE определен как %d\n", PAGESIZE+0);
#else
   printf("\nидентификатор PAGESIZE не найден\n");
#endif  
#ifdef _SC_PAGESIZE
   pr_sysconf("PAGESIZE =", _SC_PAGESIZE);
#else
   printf("\nидентификатор _SC_PAGESIZE не найден\n");
#endif

#ifdef PAGE_SIZE
   printf("\nPAGE_SIZE определен как %d\n", PAGE_SIZE+0);
#else
   printf("\nидентификатор PAGE_SIZE не найден\n");
#endif  
#ifdef _SC_PAGE_SIZE
   pr_sysconf("PAGE_SIZE =", _SC_PAGE_SIZE);
#else
   printf("\nидентификатор _SC_PAGE_SIZE не найден\n");
#endif

#ifdef RE_DUP_MAX
   printf("\nRE_DUP_MAX определен как %d\n", RE_DUP_MAX+0);
#else
   printf("\nидентификатор RE_DUP_MAX не найден\n");
#endif  
#ifdef _SC_RE_DUP_MAX
   pr_sysconf("RE_DUP_MAX =", _SC_RE_DUP_MAX);
#else
   printf("\nидентификатор _SC_RE_DUP_MAX не найден\n");
#endif

#ifdef RTSIG_MAX
   printf("\nRTSIG_MAX определен как %d\n", RTSIG_MAX+0);
#else
   printf("\nидентификатор RTSIG_MAX не найден\n");
#endif  
#ifdef _SC_RTSIG_MAX
   pr_sysconf("RTSIG_MAX =", _SC_RTSIG_MAX);
#else
   printf("\nидентификатор _SC_RTSIG_MAX не найден\n");
#endif

#ifdef SEM_NSEMS_MAX
   printf("\nSEM_NSEMS_MAX определен как %d\n", SEM_NSEMS_MAX+0);
#else
   printf("\nидентификатор SEM_NSEMS_MAX не найден\n");
#endif  
#ifdef _SC_SEM_NSEMS_MAX
   pr_sysconf("SEM_NSEMS_MAX =", _SC_SEM_NSEMS_MAX);
#else
   printf("\nидентификатор _SC_SEM_NSEMS_MAX не найден\n");
#endif

#ifdef SEM_VALUE_MAX
   printf("\nSEM_VALUE_MAX определен как %d\n", SEM_VALUE_MAX+0);
#else
   printf("\nидентификатор SEM_VALUE_MAX не найден\n");
#endif  
#ifdef _SC_SEM_VALUE_MAX
   pr_sysconf("SEM_VALUE_MAX =", _SC_SEM_VALUE_MAX);
#else
   printf("\nидентификатор _SC_SEM_VALUE_MAX не найден\n");
#endif

#ifdef SIGQUEUE_MAX
   printf("\nSIGQUEUE_MAX определен как %d\n", SIGQUEUE_MAX+0);
#else
   printf("\nидентификатор SIGQUEUE_MAX не найден\n");
#endif  
#ifdef _SC_SIGQUEUE_MAX
   pr_sysconf("SIGQUEUE_MAX =", _SC_SIGQUEUE_MAX);
#else
   printf("\nидентификатор _SC_SIGQUEUE_MAX не найден\n");
#endif

#ifdef STREAM_MAX
   printf("\nSTREAM_MAX определен как %d\n", STREAM_MAX+0);
#else
   printf("\nидентификатор STREAM_MAX не найден\n");
#endif  
#ifdef _SC_STREAM_MAX
   pr_sysconf("STREAM_MAX =", _SC_STREAM_MAX);
#else
   printf("\nидентификатор _SC_STREAM_MAX не найден\n");
#endif

#ifdef SYMLOOP_MAX
   printf("\nSYMLOOP_MAX определен как %d\n", SYMLOOP_MAX+0);
#else
   printf("\nидентификатор SYMLOOP_MAX не найден\n");
#endif  
#ifdef _SC_SYMLOOP_MAX
   pr_sysconf("SYMLOOP_MAX =", _SC_SYMLOOP_MAX);
#else
   printf("\nидентификатор _SC_SYMLOOP_MAX не найден\n");
#endif

#ifdef TIMER_MAX
   printf("\nTIMER_MAX определен как %d\n", TIMER_MAX+0);
#else
   printf("\nидентификатор TIMER_MAX не найден\n");
#endif  
#ifdef _SC_TIMER_MAX
   pr_sysconf("TIMER_MAX =", _SC_TIMER_MAX);
#else
   printf("\nидентификатор _SC_TIMER_MAX не найден\n");
#endif

#ifdef TTY_NAME_MAX
   printf("\nTTY_NAME_MAX определен как %d\n", TTY_NAME_MAX+0);
#else
   printf("\nидентификатор TTY_NAME_MAX не найден\n");
#endif  
#ifdef _SC_TTY_NAME_MAX
   pr_sysconf("TTY_NAME_MAX =", _SC_TTY_NAME_MAX);
#else
   printf("\nидентификатор _SC_TTY_NAME_MAX не найден\n");
#endif

#ifdef TZNAME_MAX
   printf("\nTZNAME_MAX определен как %d\n", TZNAME_MAX+0);
#else
   printf("\nидентификатор TZNAME_MAX не найден\n");
#endif  
#ifdef _SC_TZNAME_MAX
   pr_sysconf("TZNAME_MAX =", _SC_TZNAME_MAX);
#else
   printf("\nидентификатор _SC_TZNAME_MAX не найден\n");
#endif

printf("\nНеобязательные параметры, которые могут быть определены с помощью функции sysconf():\n");
#ifdef _POSIX_ADVISORY_INFO
   printf("\n_POSIX_ADVISORY_INFO определен как %ld\n", _POSIX_ADVISORY_INFO+0);
#else
   printf("\nидентификатор _POSIX_ADVISORY_INFO не найден\n");
#endif  
#ifdef _SC_ADVISORY_INFO
   pr_sysconf("_POSIX_ADVISORY_INFO =", _SC_ADVISORY_INFO);
#else
   printf("\nидентификатор _SC_ADVISORY_INFO не найден\n");
#endif 

#ifdef _POSIX_CPUTIME
   printf("\n_POSIX_CPUTIME определен как %d\n", _POSIX_CPUTIME+0);
#else
   printf("\nидентификатор _POSIX_CPUTIME не найден\n");
#endif  
#ifdef _SC_CPUTIME
   pr_sysconf("_POSIX_CPUTIME =", _SC_CPUTIME);
#else
   printf("\nидентификатор _SC_CPUTIME не найден\n");
#endif

#ifdef _POSIX_FSYNC
   printf("\n_POSIX_FSYNC определен как %ld\n", _POSIX_FSYNC+0);
#else
   printf("\nидентификатор _POSIX_FSYNC не найден\n");
#endif  
#ifdef _SC_FSYNC
   pr_sysconf("_POSIX_FSYNC =", _SC_FSYNC);
#else
   printf("\nидентификатор _SC_FSYNC не найден\n");
#endif

#ifdef _POSIX_IPV6
   printf("\n_POSIX_IPV6 определен как %ld\n", _POSIX_IPV6+0);
#else
   printf("\nидентификатор _POSIX_IPV6 не найден\n");
#endif  
#ifdef _SC_IPV6
   pr_sysconf("_POSIX_IPV6 =", _SC_IPV6);
#else
   printf("\nидентификатор _SC_IPV6 не найден\n");
#endif

#ifdef _POSIX_MEMLOCK
   printf("\n_POSIX_MEMLOCK определен как %ld\n", _POSIX_MEMLOCK+0);
#else
   printf("\nидентификатор _POSIX_MEMLOCK не найден\n");
#endif  
#ifdef _SC_MEMLOCK
   pr_sysconf("_POSIX_MEMLOCK =", _SC_MEMLOCK);
#else
   printf("\nидентификатор _SC_MEMLOCK не найден\n");
#endif

#ifdef _POSIX_MEMLOCK_RANGE
   printf("\n_POSIX_MEMLOCK_RANGE определен как %ld\n", _POSIX_MEMLOCK_RANGE+0);
#else
   printf("\nидентификатор _POSIX_MEMLOCK_RANGE не найден\n");
#endif  
#ifdef _SC_MEMLOCK_RANGE
   pr_sysconf("_POSIX_MEMLOCK_RANGE =", _SC_MEMLOCK_RANGE);
#else
   printf("\nидентификатор _SC_MEMLOCK_RANGE не найден\n");
#endif

#ifdef _POSIX_RAW_SOCKETS
   printf("\n_POSIX_RAW_SOCKETS определен как %ld\n", _POSIX_RAW_SOCKETS+0);
#else
   printf("\nидентификатор _POSIX_RAW_SOCKETS не найден\n");
#endif  
#ifdef _SC_RAW_SOCKETS
   pr_sysconf("_POSIX_RAW_SOCKETS =", _SC_RAW_SOCKETS);
#else
   printf("\nидентификатор _SC_RAW_SOCKETS не найден\n");
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
   
   
               
