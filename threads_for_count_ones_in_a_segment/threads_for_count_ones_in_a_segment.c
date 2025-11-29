// threads_for_count_ones_in_a_segment.c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <pthread.h>
#define THREADS 500
uint64_t countOnes( uint64_t left, uint64_t right );
uint64_t countOnes_1( uint64_t left, uint64_t right );

struct range{
   uint64_t left;
   uint64_t right;
   uint64_t count_1_r;
};

struct range arr_r[THREADS];
uint64_t step = 2000000;
void *thr_fun(void *arg)
{
  uint64_t temp = ((struct range*)arg)->left;
  uint64_t count_1 = 0;
  uint64_t temp_count_1 = 0;
  uint64_t count_0 = 0;
  while (temp)
  {
    if ((temp & 1) == 1)
        temp_count_1++;
        temp >>= 1;
  }
  count_1 = count_1 + temp_count_1;
  ((struct range*)arg)->left++;
  while (((struct range*)arg)->left <= ((struct range*)arg)->right)
  {
    if (((struct range*)arg)->left % 2 == 0)
    {
        temp = ((struct range*)arg)->left;
        count_0 = 0;
        while (temp)
        {
            if ((temp & 1) == 0)
                count_0++;
            else
                break;
            temp >>= 1;
        }
        temp_count_1 = temp_count_1 - count_0 + 1;
        count_1 = count_1 + temp_count_1;
    }
    else
    {
        temp_count_1 = temp_count_1 + 1;
        count_1 = count_1 + temp_count_1;
    }
    if (((struct range*)arg)->left < ((struct range*)arg)->right)
        ((struct range*)arg)->left++;
    else
        break;
  }
  ((struct range*)arg)->count_1_r = count_1;
  return count_1;
}
void *thr_fun_1(void *arg)
{
  uint64_t temp = 0;
  while (((struct range*)arg)->left <= ((struct range*)arg)->right)
  {
    temp = ((struct range*)arg)->left;
    while (temp)
    {
      if ((temp & 1) == 1)
         ((struct range*)arg)->count_1_r++;
      temp >>= 1;
    }
    if (((struct range*)arg)->left < ((struct range*)arg)->right)
        ((struct range*)arg)->left++;
    else
        break;
  }
  return (void *)0;
}

int main(void)
{

    uint64_t right = 18446744073709551615;


    //printf("%llu\n", countOnes_1(right - step * 500, right));
    printf("%llu\n", countOnes_1(4250829, 231192380));
    printf("Hello world!\n");
    return 0;
}
uint64_t countOnes_1( uint64_t left, uint64_t right )
{
  uint64_t count_1 = 0;
  pthread_t arr_tid[THREADS];
  uint64_t count_tid = 0;

  for (count_tid = 0; right - left > step * 80; count_tid++)
  {
      arr_r[count_tid].right = right;
      arr_r[count_tid].left = right - step + 1;
      if (pthread_create(&arr_tid[count_tid], NULL, thr_fun, (void *)&arr_r[count_tid]) != 0)
          printf("Не удалось создать поток");
      right = right - step;
  }
  /*
  uint64_t temp = left;
  while (left <= right)
  {
    temp = left;
    while (temp)
    {
      if ((temp & 1) == 1)
         count_1++;
      temp >>= 1;
    }
    if (left < right)
        left++;
    else
        break;
  }
  */
  uint64_t temp = left;
  uint64_t temp_count_1 = 0;
  uint64_t count_0 = 0;
  temp = left;
  temp_count_1 = 0;
  while (temp)
  {
    if ((temp & 1) == 1)
        temp_count_1++;
        temp >>= 1;
  }
  count_1 = count_1 + temp_count_1;
  left++;
  while (left <= right)
  {
    if (left % 2 == 0)
    {
        temp = left;
        count_0 = 0;
        while (temp)
        {
            if ((temp & 1) == 0)
                count_0++;
            else
                break;
            temp >>= 1;
        }
        temp_count_1 = temp_count_1 - count_0 + 1;
        count_1 = count_1 + temp_count_1;
    }
    else
    {
        temp_count_1 = temp_count_1 + 1;
        count_1 = count_1 + temp_count_1;
    }
    if (left < right)
        left++;
    else
        break;
  }
  //sleep(1);
  for (uint64_t i = 0; i < count_tid; i++)
  {
    count_1 = count_1 + arr_r[i].count_1_r;
  }
  return count_1;
}
uint64_t countOnes( uint64_t left, uint64_t right )
{
  uint64_t array[64] = {0};
  uint64_t count_1 = 0;
  array[0] = 1;
  array[1] = 1 + array[0];
  array[1] = 2;
  array[2] = 1 + array[0] + array[1] + (array[0] + array[1]);
  return count_1;
}

