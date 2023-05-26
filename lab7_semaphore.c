#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define LED_COUNT 4

sem_t semaphore;
int counter = 2;

typedef struct {
	int gpio;
	int stat;
} Swamp;

void* child(void *_swamp){
	Swamp *swamp = (Swamp *)_swamp;
	int i = counter;
	while(i--) {
		int temp = sem_wait(&semaphore);
		printf("GPIO:%d i = %d\n",swamp->gpio, i);
		printf("GPIO:%d status:%d\n", swamp->gpio, swamp->stat);
		//printf("GPIO:%d count: %d\n", ++count);
		//printf("count: %d\n", temp);
	}
	free(swamp);
    //printf("GPIO:\n");
    pthread_exit(NULL);
}

//  int pthread_create(pthread_t *restrict thread,
//                         const pthread_attr_t *restrict attr,
//                         void *(*start_routine)(void *),
//                         void *restrict arg);

int main(int argc,char* argv[]){
    sem_init(&semaphore,0, LED_COUNT);
    pthread_t t;
	for (int i = 0; i < LED_COUNT; ++i) {
		Swamp *swamp = malloc(sizeof(Swamp));
		swamp->gpio = i;
		swamp->stat = 0;
		pthread_create(&t,NULL,child,swamp);
	}

    // printf("status:%c%c%c%c\n",&argv[0],&argv[1],&argv[2],&argv[3]);

	sem_post(&semaphore);
    sleep(1);
	printf("sleep");
	sem_post(&semaphore);
//	for (int i = 0; i < LED_COUNT; ++i) {
//		sem_post(&semaphore);
//		sem_post(&semaphore);
//	}

    pthread_join(t,NULL);
    return 0;
}
