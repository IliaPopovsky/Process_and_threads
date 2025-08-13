// incorrect_use_of_function_argument_pthread_exit()  Некорректное использование аргумента функции pthread_exit()
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

struct foo{
    int a, b, c, d;
};

void printfoo(const char *s, const struct foo *fp)
{
   printf("%s", s);
   printf(" структура по адресу 0x%1lx\n", (unsigned long)fp);
   printf(" foo.a = %d\n", fp->a);
   printf(" foo.b = %d\n", fp->b);
   printf(" foo.c = %d\n", fp->c);
   printf(" foo.d = %d\n", fp->d);   
}

void *thr_fn1(void *arg)
{
   struct foo foolocal = {1, 2, 3, 4};
   
   printfoo("поток 1:\n", &foolocal);
   pthread_exit((void *)&foolocal);
}

void *thr_fn2(void *arg)
{
   printf("поток 2: идентификатор - %ld\n", (unsigned long)pthread_self());
   pthread_exit((void *)0);
}

int main(void)
{
   int err;
   pthread_t tid1, tid2;
   struct foo *fp;
   
   err = pthread_create(&tid1, NULL, thr_fn1, NULL);
   if(err != 0)
      err_exit(err, "невозможно создать поток 1");
   err = pthread_join(tid1, (void *)&fp);
   if(err != 0)
      err_exit(err, "невозможно присоединить поток 1");
   sleep(1);
   printf("родительский процесс создает второй поток\n");
   err = pthread_create(&tid2, NULL, thr_fn2, NULL);               // стек потока 2 накладывается на стек потока 1, это происходит и в Linux, и в Mac OS X, и в FreeBSD 
   if(err != 0)
      err_exit(err, "невозможно создать поток 2");
   sleep(1);
   printfoo("родительский процесс:\n", fp);
   exit(0);
}
