// use_pthread_mutex_timedlock  Использование функции pthread_mutex_timedlock()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int main(void)
{ 
	int err;
	struct timespec tout;
	struct tm *tmp;
	char buf[64];
	
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_lock(&lock);
	printf("мьютекс заперт\n");
	clock_gettime(CLOCK_REALTIME, &tout);                           // возвращает текущее время в tout
	tmp = localtime(&tout.tv_sec);
	strftime(buf, sizeof(buf), "%r", tmp);
	printf("текущее время: %s\n", buf);
	tout.tv_sec += 10; /* 10 секунд, начиная от текущего времени */
	/* внимание: это может привести к тупиковой ситуации */
	err = pthread_mutex_timedlock(&lock, &tout);
	clock_gettime(CLOCK_REALTIME, &tout);                          // возвращает текущее время в tout
	tmp = localtime(&tout.tv_sec);
	strftime(buf, sizeof(buf), "%r", tmp);
	printf("текущее время: %s\n", buf);
	if (err == 0)
	    printf("мьютекс снова заперт!\n");
	else
	    printf("не получилось повторно запереть мьютекс: %s\n", strerror(err));
	exit(0);
}
