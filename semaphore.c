#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define THREAD_COUNT 4

sem_t semaphore;

typedef struct {
  int i;
} Data;

void *callback(void *arg) {
  sem_wait(&semaphore);

  Data *data = (Data *)arg;

  printf("%d\n", data->i);
  sleep(2);

  sem_post(&semaphore);

  return NULL;
}

int main() {
  int i;
  pthread_t threads[THREAD_COUNT];
  Data data[THREAD_COUNT];
  sem_init(&semaphore, 0, 1);

  for (i = 0; i < THREAD_COUNT; ++i) {
    data[i].i = i;
    pthread_create(&threads[i], NULL, callback, &data[i]);
    sleep(1);
  }

  for (i = 0; i < THREAD_COUNT; ++i) {
    pthread_join(threads[i], NULL);
  }

  sem_destroy(&semaphore);

  return 0;
}
