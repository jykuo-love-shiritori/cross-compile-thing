#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

sem_t semaphore;

void* child(){
    sem_wait(&semaphore);
    print("GPIO:%d status:%c",);
    sleep(1);
    pthread_exit(NULL);
}

int main(int argc,char* argv[]){
    sem_init(&semaphore,0,0);
    pthread_t t;
    pthread_create(&t,NULL,child,NULL);
    print("status:%c%c%c%c",&argv[0],&argv[1],&argv[2],&argv[3]);
    sem_post(&semaphore);
    sem_post(&semaphore);
    sem_post(&semaphore);
    sem_post(&semaphore);

    pthread_join(t,NULL);
    return 0;
}