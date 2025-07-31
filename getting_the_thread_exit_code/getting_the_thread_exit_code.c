// getting_the_thread_exit_code   Получение кода завершения (выхода) потока
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <errno.h> /* определение переменной errno */
#include <stdarg.h> /* список аргументов переменной длины ISO C */

#define MAXLINE 4096 /* максимальная длина строки */

/*
 * Выводит сообщение и возвращает управление в вызывающую функцию.
 * Вызывающая функция определяет значение флага "errnoflag".
 */
static void err_doit(int errnoflag, int error, const char *fmt, va_list ap)
{
   char buf[MAXLINE];
   vsnprintf(buf, MAXLINE-1, fmt, ap);
   if (errnoflag)
   snprintf(buf+strlen(buf), MAXLINE-strlen(buf)-1, ": %s",
   strerror(error));
   strcat(buf, "\n");
   fflush(stdout); /* в случае, когда stdout и stderr - одно и то же устройство */
   fputs(buf, stderr);
   fflush(NULL); /* сбрасывает все выходные потоки */
}

/*
 * Обрабатывает фатальные ошибки, не связанные с системными вызовами.
 * Код ошибки передается в аргументе.
 * Выводит сообщение и завершает работу процесса.
 */
void err_exit(int error, const char *fmt, ...)
{
   va_list ap;
   va_start(ap, fmt);
   err_doit(1, error, fmt, ap);
   va_end(ap);
   exit(1);
}

void *thr_fn1(void *arg)
{
   printf("поток 1: выход\n");
   return((void *)1);
}

void *thr_fn2(void *arg)
{
   printf("поток 2: выход\n");
   pthread_exit((void *)2);
}

int main(void)
{
   int err;
   pthread_t tid1, tid2;
   void *tret;
   
   err = pthread_create(&tid1, NULL, thr_fn1, NULL);
   if(err != 0)
      err_exit(err, "невозможно создать поток 1");
   err = pthread_create(&tid2, NULL, thr_fn2, NULL);
   if(err != 0)
      err_exit(err, "невозможно создать поток 2");
   err = pthread_join(tid1, &tret);
   if(err != 0)
      err_exit(err, "невозможно присоединить поток 1");
   printf("код выхода потока 1: %ld\n", (long)tret);
   err = pthread_join(tid2, &tret);
   if(err != 0)
      err_exit(err, "невозможно присоединить поток 2");
   printf("код выхода потока 2: %ld\n", (long)tret);
   exit(0);
}  
