// eliminated_the_data_race_but_the_race_condition_remained.c
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>

#define SH_SIZE 4

// Разделяемый файловый дескриптор, с помощью которого мы ссылаемся на объект в разделяемой памяти
int shared_fd = -1;

// Указатель на разделяемый счетчик
int32_t *counter = NULL;

// Указатель на разделяемы семафор
sem_t *semaphore = NULL;

void init_control_mechanism(){
    semaphore = sem_open("/sem0", O_CREAT | O_EXCL, 0600, 1);
    if(semaphore == SEM_FAILED){
       fprintf(stderr, "ERROR: Opening the semaphore failed: %s\n", strerror(errno));
       exit(1);
    }
}

void shutdown_control_mechanism(){
    if(sem_close(semaphore) < 0){
       fprintf(stderr, "ERROR: Closing the semaphore failed: %s\n", strerror(errno));
       exit(1); 
    }
    if(sem_unlink("/sem0") < 0){
       fprintf(stderr, "ERROR: Unlinking failed: %s\n", strerror(errno));
       exit(1);
    }
}

void init_shared_resource(){
   // Открываем объект в разделяемой памяти
   shared_fd = shm_open("/shm0", O_CREAT | O_RDWR, 0600);
   if(shared_fd < 0){
       fprintf(stderr, "ERROR: Failed to create shared memory: %s\n", strerror(errno));
       exit(1);
   }
   fprintf(stdout, "Shared memory is created with fd: %d\n", shared_fd);
}

void shutdown_shared_resource(){
   if(shm_unlink("/shm0") < 0){
       fprintf(stderr, "ERROR: Unlinking shared memory failed: %s\n", strerror(errno));
       exit(1);
   }
}
    
void inc_counter(){
   usleep(1);
   // Вход на критический участок
   sem_wait(semaphore);             // Возвращаемое значение должно проверяться
   int32_t temp = *counter;
   usleep(1);
   temp++;
   usleep(1);
   *counter = temp;
   sem_post(semaphore);             // Возвращаемое значение должно проверяться
   usleep(1);
}

int main(int argc, char **argv){
   
   // Родительский процесс должен инициализировать разделяемый ресурс
   init_shared_resource();
   
   // Родительский процесс должен инициализировать механизм управления
   init_control_mechanism();
   
   // Выделяем и усекаем разделяемую область памяти
   if(ftruncate(shared_fd, SH_SIZE * sizeof(char)) < 0){
       fprintf(stderr, "ERROR: Truncation failed: %s\n", strerror(errno));
       return 1;
   }
   fprintf(stdout, "The memory region is truncated.\n");
   
   // Отражаем разделяемую память и инициализируем счетчик
   void *map = mmap(0, SH_SIZE, PROT_WRITE, MAP_SHARED, shared_fd, 0);
   if(map == MAP_FAILED){
       fprintf(stderr, "ERROR; Mapping failed: %s\n", strerror(errno));
       return 1;
   }
   counter = (int32_t *)map;
   *counter = 0;
   
   // Создаем новый процесс из текущего
   pid_t pid = fork();
   if(pid){ // Родительский процесс
     // Инкрементируем счетчик
     inc_counter();
     fprintf(stdout, "The parent process sees the counter as %d.\n", *counter);
     
     // Ждем завершения дочернего процесса
     int status = -1;
     wait(&status);
     fprintf(stdout, "The child process finished with status %d.\n", status);
   }
   else{ // Дочерний процесс
     // Инкрементируем счетчик
     inc_counter();
     fprintf(stdout, "The child process sees the counter as %d.\n", *counter);
   }
   
   // Оба процесса должны уничтожить отражение в области памяти и закрыть свои файловые дескрипторы
   if(munmap(counter, SH_SIZE) < 0){
     fprintf(stderr, "ERROR: Unmapping failed: %s\n", strerror(errno));
     return 1;
   }
   if(close(shared_fd) < 0){
     fprintf(stderr, "ERROR: Closing shared memory fd filed: %s\n", strerror(errno));
     return 1;
   }
   
   // Только родительскому процессу нужно закрывать разделяемый ресурс и задействованый механизм управления
   if(pid){
     shutdown_shared_resource();
     shutdown_control_mechanism();
   }
   return 0;
}
