// use_thread_specific_data.c  Потокобезопасная реализация strerror() с использованием локальных данных потока

#include <stdlib.h>
#include <stdio.h>
#include <string.h> /* Получаем объявление strerror() */
#include <pthread.h>
#include <errno.h>
#include <stdarg.h>
#include "ename.c.inc"

static pthread_once_t once = PTHREAD_ONCE_INIT;
static pthread_key_t strerrorKey;
#define MAX_ERROR_LEN 256 /* Максимальная длина строки буфера локальных данных потока, возвращаемого функцией strerror() */

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

static void /* Освобождаем буфер с локальными данными потока */
destructor(void *buf)
{
   free(buf);
}

static void /* Функция для единовременного создания ключа */
createKey(void)
{
   int s;
   /* На уровне потока выделяем уникальный ключ для буфера
      и сохраняем адрес деструктора этого буфера */
   s = pthread_key_create(&strerrorKey, destructor);
   if (s != 0)
       err_exit(s, "pthread_key_create");
}

char *
strerror(int err)
{
   int s;
   char *buf;
   /* Выделяем ключ для данных уровня потока при первом вызове */
   s = pthread_once(&once, createKey);
   if (s != 0)
       err_exit(s, "pthread_once");
   buf = pthread_getspecific(strerrorKey);
   if (buf == NULL) { /* Если это первый вызов из данного потока,
                         выделяем буфер и сохраняем его адрес */
       buf = malloc(MAX_ERROR_LEN);
       if (buf == NULL)
           err_exit(errno, "malloc");
       s = pthread_setspecific(strerrorKey, buf);
       if (s != 0)
           err_exit(s,"pthread_setspecific");
   }
   
   if (err < 0 || err >= MAX_ENAME || ename[err] == NULL) {
        snprintf(buf, MAX_ERROR_LEN, "Unknown error %d", err);
   } else {
        strncpy(buf, ename[err], MAX_ERROR_LEN - 1);
        buf[MAX_ERROR_LEN - 1] = '\0';          /* Завершаем строку символом '\0' */
   }
   return buf;
}

static void *
threadFunc(void *arg)
{
    char *str;

    printf("Other thread about to call strerror()\n");
    str = strerror(EPERM);
    printf("Other thread: str (%p) = %s\n", str, str);

    return NULL;
}

int
main(int argc, char *argv[])
{
    pthread_t t;
    int s;
    char *str;

    str = strerror(EINVAL);
    printf("Main thread has called strerror()\n");

    s = pthread_create(&t, NULL, threadFunc, NULL);
    if (s != 0)
        err_exit(s, "pthread_create");

    s = pthread_join(t, NULL);
    if (s != 0)
        err_exit(s, "pthread_join");

    printf("Main thread:  str (%p) = %s\n", str, str);

    exit(EXIT_SUCCESS);
}
