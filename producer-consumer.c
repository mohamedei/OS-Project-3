#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

pthread_mutex_t mutex;
sem_t full, empty;
int count, in, out;

#include "buffer.h"

void *consumer(void *param);
void *producer(void *param);

int main(int argc, char **argv){
  if (argc != 4){
    printf("ERROR\n");
    exit(1);
  }

  const long int stime = strtol(argv[1], NULL, 0);
  const long int num_prod = strtol(argv[2], NULL, 0);
  const long int num_cons = strtol(argv[3], NULL, 0);

  srand(time(NULL));
  pthread_mutex_init(&mutex, NULL);
  sem_init(&empty, 0, BUFFER_SIZE); 
  sem_init(&full, 0, 0);
  count = in = out = 0;

  pthread_t prod[num_prod];
  pthread_t cons[num_cons];
  
  int i;
  for(i = 0; i < num_prod; i++)
    pthread_create(&prod[i], NULL, producer, NULL);
  for(i = 0; i < num_cons; i++)
    pthread_create(&cons[i], NULL, consumer, NULL);

  sleep(stime);
  return 0;
}

void *producer(void *param){
  buffer_item item;
  while(1){
    sleep(rand() % 5 + 1); 
    item = rand();
    if(insert_item(item))
      printf("Error occured\n");
    else
      printf("Produced %d\n", item);
  }
}

void *consumer(void *param){
  buffer_item item;
  while(1){
    sleep(rand() % 5 + 1);
    if(remove_item(&item))
      printf("Error occured\n");
    else
      printf("Consumed %d\n", item);
  }
}
