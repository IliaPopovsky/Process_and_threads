// process_management.c
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
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
 * Nonfatal error related to a system call.
 * Print a message and return.
 */
void
err_ret(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(1, errno, fmt, ap);
	va_end(ap);
}

int main(void)
{
   char buf[MAXLINE];
   pid_t pid;
   int status;
   
   printf("%% ");   // вывести приглашение
   while (fgets(buf, MAXLINE, stdin) != NULL)
   {
      if (buf[strlen(buf) - 1] == '\n')
          buf[strlen(buf) - 1] = 0;      // заменить символ перевода строки
      if ((pid = fork()) < 0) {
          err_sys("ошибка вызова fork");
      } else if (pid == 0) {             // дочерний процесс
          execlp(buf, buf, (char *)0);
          err_ret("невозможно выполнить: %s", buf);
          exit(127);
      }
          
      /* родительский процесс */
      if ((pid = waitpid(pid, &status, 0)) < 0)
          err_sys("ошибка вызова waitpid");
      printf("%% ");   // вывести приглашение
   } 
   
  
   exit(0);
}
