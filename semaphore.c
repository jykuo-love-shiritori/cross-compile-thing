#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "util.h"

#define MAX_THREAD_COUNT 50

#define PIN0 396
#define PIN1 392
#define PIN2 255
#define PIN3 254

sem_t semaphore;

typedef struct {
  char state[4];
} Data;

void *callback(void *arg) {
  sem_wait(&semaphore);

  char *temp = "0";

  Data *data = (Data *)arg;

  temp[0] = data->state[0];
  gpio_set_value(PIN0, temp);
  temp[0] = data->state[1];
  gpio_set_value(PIN1, temp);
  temp[0] = data->state[2];
  gpio_set_value(PIN2, temp);
  temp[0] = data->state[3];
  gpio_set_value(PIN3, temp);

  sem_post(&semaphore);

  return NULL;
}

int main(int argc, char **argv) {
  int i;
  int loop_count = atoi(argv[2]);
  char init[4] = "1234";
  memcpy(init, argv[1], 4 * sizeof(char));

  gpio_export(PIN0);
  gpio_export(PIN1);
  gpio_export(PIN2);
  gpio_export(PIN3);

  gpio_set_dir(PIN0, "out");
  gpio_set_dir(PIN1, "out");
  gpio_set_dir(PIN2, "out");
  gpio_set_dir(PIN3, "out");

  printf("%d\n", loop_count);

  pthread_t threads[MAX_THREAD_COUNT];
  Data data[MAX_THREAD_COUNT];
  sem_init(&semaphore, 0, 1);

  for (i = 0; i < loop_count; ++i) {
    memcpy(data[i].state, init, 4 * sizeof(char));
    pthread_create(&threads[i], NULL, callback, &data[i]);

    sleep(1);

	  for (int j = 0; j < 4; ++j) {
		  init[j] ^= 1;
	  }
  }

  for (i = 0; i < loop_count; ++i) {
    pthread_join(threads[i], NULL);
  }

  sem_destroy(&semaphore);

  gpio_unexport(PIN0);
  gpio_unexport(PIN1);
  gpio_unexport(PIN2);
  gpio_unexport(PIN3);

  return 0;
}
