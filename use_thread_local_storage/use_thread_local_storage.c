// use_thread_local_storage.c  Использование локального хранилища потока
/* По аналогии с локальными данными потока(thread-specific data) локальное хранилище потока(thread-local storage) предоставляет средство постоянного хранения информации потока. Это нестандартная концепция, но она реализована разных системах UNIX (таких как Linux, FreeBSD). 
   Главное преимущество локального хранилища потока(thread-local storage) заключается в том, что его значительно проще использовать по сравнению с локальными данными потока(thread-specific data).
Чтобы сделать глобальную или статическую переменную локальной для каждого потока, нужно просто указать при ее объявлении спецификатор __thread:  
static __thread char buf[MAX_ERROR_LEN];
   Каждый поток имеет свою собственную копию переменных, объявленных с этим спецификатором. Переменные в локальном хранилище потока(thread-local storage) существуют до тех пор, пока не завершится поток, который ими владеет - в этот момент локальном хранилище потока(thread-local storage) автоматически утилизируется. 
   Стоит отметить следующие моменты касательно объявления и использования локальных переменных потока:
- Ключевое слово __thread должно быть указано сразу после ключевых слов static или extern, если какое либо из них используется при объявлении.
- Объявление локальной переменной потока может содержать инициализатор - по аналогии с тем, как это делается в обычных глобальных или статических переменных.
- Оператор адреса в языке C (&) может использоваться для получения указателя на локальную переменную потока.
   Локальное хранилище потока(thread-local storage) требует поддержки со стороны ядра (предоставляется, начиная с Linux 2.6), реализации Pthreads (предоставляется библиотекой NPTL) и компилятора языка C (обеспечивается компилятором gcc версии 3.3 и выше).  */
#include <stdlib.h>
#include <stdio.h>
#include <string.h> /* Получаем объявление strerror() */
#include <pthread.h>
#include <errno.h>
#include <stdarg.h>
#include "ename.c.inc"

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

static __thread char buf[MAX_ERROR_LEN];    /* Итоговый локальный буфер потока */

char *
strerror(int err)
{
   
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

