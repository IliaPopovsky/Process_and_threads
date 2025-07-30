// output_TID Вывод идентификаторов потоков
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <errno.h> /* определение переменной errno */
#include <stdarg.h> /* список аргументов переменной длины ISO C */

#define MAXLINE 4096 /* максимальная длина строки */

pthread_t ntid;

/*
 * Выводит сообщение и возвращает управление в вызывающую функцию.
 * Вызывающая функция определяет значение флага "errnoflag".
 */
static void
err_doit(int errnoflag, int error, const char *fmt, va_list ap)
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
void
err_exit(int error, const char *fmt, ...)
{
 va_list ap;
 va_start(ap, fmt);
 err_doit(1, error, fmt, ap);
 va_end(ap);
 exit(1);
}

void printids(const char *s)
{
   pid_t pid;
   pthread_t tid;
   
   pid = getpid();                  // Процесс может получить собсвенный идентификатор, вызвав функцию getpid()
   tid = pthread_self();            // Поток может получить собсвенный идентификатор, вызвав функцию pthread_self()
                                    // Новый поток не может использовать значение глобальной переменной ntid в качестве идентификатора, т.к. если он получит управление первым, еще до того, как функция
                                    // pthread_create вернет управление в основной поток, вместо идентификатора новый поток обнаружит неинициализированное значение глобальной переменной ntid.
   printf("%s pid %u tid %u (0x%x)\n", s, (unsigned int)pid, (unsigned int)tid, (unsigned int)tid);
}

void *thr_fn(void *arg)
{
   printids("новый поток: ");
   return ((void *)0);
}

int main(void)
{
   int err;
   
   err = pthread_create(&ntid, NULL, thr_fn, NULL);   // &ntid - это указатель на область памяти, где будет размещен идентификатор созданного потока, если вызов функции pthread_create() завершится
                                                      // успехом
                                                      // При создании нового потока нельзя заранее предположить, кто первым получит управление - вновь созданный поток или поток, вызвавший функцию
                                                      // pthread_create()
   if(err != 0)
      err_exit(err, "невозможно создать поток");
   printids("главный поток:");
   sleep(1);
   exit(0);
}
   
