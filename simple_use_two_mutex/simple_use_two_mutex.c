// simple_use_two_mutex  Простой пример использования двух мьютексов
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


#define NHASH 29
#define HASH(id) (((unsigned long)id)%NHASH)

struct foo *fh[NHASH];
pthread_mutex_t hashlock = PTHREAD_MUTEX_INITIALIZER;

struct foo {
    int             f_count;     /* защищается мьютексом hashlock */
    pthread_mutex_t f_lock;
    int             f_id;
    struct foo      *f_next;     /* защищается мьютексом hashlock */
};

struct foo *foo_alloc(int id)     /* размещает объект в динамической памяти */
{
    struct foo *fp;
    int        idx;
    
    if ((fp = malloc(sizeof(struct foo))) != NULL) {
        fp->f_count = 1;
        fp->f_id = id;
        if (pthread_mutex_init(&fp->f_lock, NULL) != 0) {
            free(fp);
            return(NULL);
        }
        idx = HASH(id);
        pthread_mutex_lock(&hashlock);
        fp->f_next = fh[idx];
        fh[idx] = fp;
        pthread_mutex_lock(&fp->f_lock);
        pthread_mutex_unlock(&hashlock);
        /* ... продолжение инициализации ... */
        pthread_mutex_unlock(&fp->f_lock);
    }
    return (fp);
}

void foo_hold(struct foo *fp)     /* добавляет ссылку на объект */
{ 
    pthread_mutex_lock(&hashlock);
    fp->f_count++;
    pthread_mutex_unlock(&hashlock);
}

struct foo *foo_find(int id)      /* находит существующий объект */
{
    struct foo *fp;
    
    pthread_mutex_lock(&hashlock);
    for (fp = fh[HASH(id)]; fp != NULL; fp = fp->f_next) {
    	if (fp->f_id == id) {
            fp->f_count++;
            break;
        }
    }
    pthread_mutex_unlock(&hashlock);
    return (fp);
}

void foo_rele(struct foo *fp)   /* освобождает ссылку на объект */
{
    struct foo *tfp;
    int         idx;
    
    pthread_mutex_lock(&hashlock);
    if (--fp->f_count == 0) {  /* последняя ссылка, удалить из списка */
    	idx = HASH(fp->f_id);
    	tfp = fh[idx];
    	if (tfp == fp) {
    	    fh[idx] = fp->f_next;
    	} else {
    	    while (tfp->f_next != fp)
    	        tfp = tfp->f_next;
    	    tfp->f_next = fp->f_next;
    	}    
        pthread_mutex_unlock(&hashlock);   
        pthread_mutex_destroy(&fp->f_lock);
        free(fp);
    } else {
        pthread_mutex_unlock(&hashlock); 
    }
}

void *thr_fn1(void *arg)
{
    printf("запуск потока 1\n");
    struct foo *lfp;
    for(int i = 0; i < NHASH; i++)
    {
        if(i % 2 != 0)
        {
            lfp = foo_alloc(i);
            if(lfp == NULL)
               exit(1);
            pthread_mutex_lock(&lfp->f_lock);
            pthread_mutex_lock(&hashlock);
                printf("f_id = %d\n", lfp->f_id);
                printf("f_count = %d\n", lfp->f_count);
            pthread_mutex_unlock(&hashlock);
            pthread_mutex_unlock(&lfp->f_lock);
            
        }
    }
}

void *thr_fn2(void *arg)
{
    printf("запуск потока 2\n");
    struct foo *lfp;
    for(int i = 0; i < NHASH; i++)
    {
        if(i % 2 == 0)
        {
            lfp = foo_alloc(i);
            pthread_mutex_lock(&lfp->f_lock);
            pthread_mutex_lock(&hashlock);
                printf("f_id = %d\n", lfp->f_id);
                printf("f_count = %d\n", lfp->f_count);
            pthread_mutex_unlock(&hashlock);
            pthread_mutex_unlock(&lfp->f_lock);
            
        }
    }
}

int main(void)
{
    int err;
    pthread_t tid1, tid2;
    void *tret;
    
   err = pthread_create(&tid1, NULL, thr_fn1, (void *)1);
   if(err != 0)
      err_exit(err, "невозможно создать поток 1");
   err = pthread_create(&tid2, NULL, thr_fn2, (void *)1);
   if(err != 0)
      err_exit(err, "невозможно создать поток 2");
      
   sleep(4);
    
   for(int i = 0; i < NHASH; i++)
   {
     if(fh[i] != NULL && i % 3 == 0)
     {
        pthread_mutex_lock(&fh[i]->f_lock);
                printf("f_id = %d\n", fh[i]->f_id);
                foo_hold(fh[i]);
        pthread_mutex_lock(&hashlock);
                printf("f_count = %d\n", fh[i]->f_count);
        pthread_mutex_unlock(&hashlock);     
        pthread_mutex_unlock(&fh[i]->f_lock);
        
     }
     
   }  
      
   err = pthread_join(tid1, &tret);
   if(err != 0)
      err_exit(err, "невозможно присоединить поток 1");
   printf("код выхода потока 1: %ld\n", (long)tret);
   err = pthread_join(tid2, &tret);
   if(err != 0)
      err_exit(err, "невозможно присоединить поток 2");
   printf("код выхода потока 2: %ld\n", (long)tret);
   return 0;
}
