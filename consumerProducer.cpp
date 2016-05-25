
#include <queue>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <iostream>

using namespace std;

#define N_PROD 2
#define N_CONS 3
#define QUEUE_SIZE 10

#define MIN_TIMEOUT 500000
#define RAND_TIMEOUT 500000

void* producerTask(void *id);
void* consumerTask(void *id);
void timeout();

queue<int> resources;
int resourcesCounter;

pthread_mutex_t mutex;
pthread_cond_t notEmpty;
pthread_cond_t notFull;
pthread_mutex_t adding;
pthread_mutex_t reading;

int main(){
    resourcesCounter = 0;
    pthread_t producers[N_PROD];
    pthread_t consumers[N_CONS];
    int nCons[N_CONS], nProd[N_PROD];
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&adding, NULL);
    pthread_mutex_init(&reading, NULL);
    pthread_cond_init(&notFull, NULL);
    pthread_cond_init(&notEmpty, NULL);

    for (int i = 0; i < N_PROD; i++) {
        nProd[i]=i;
        pthread_create(&producers[i], NULL, producerTask, (void *) &nProd[i]);
    }

    for (int i = 0; i < N_CONS; i++) {
        nCons[i] = i;
        pthread_create(&consumers[i], NULL, consumerTask, (void *) &nCons[i]);
    }


    pthread_join(producers[0], NULL);

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&adding);
    pthread_mutex_destroy(&reading);
    pthread_cond_destroy(&notFull);
    pthread_cond_destroy(&notEmpty);
}

void* producerTask(void *id){
    int ID = *((int*)id);

    while (true){
        pthread_mutex_lock(&adding);
        if (resources.size() == QUEUE_SIZE){
            pthread_cond_wait(&notFull, &mutex);
        }
        resources.push(resourcesCounter);
        pthread_cond_signal(&notEmpty);
        printf("The producer #%d has pushed resource  %d .\n", ID, resourcesCounter);
        pthread_mutex_unlock(&adding);
        timeout();
    }
}

void* consumerTask(void *id){
    int ID = *((int*)id);

    while(true){
        pthread_mutex_lock(&reading);
        if (resources.empty()){
            pthread_cond_wait(&notEmpty, &mutex);
        }
        resources.pop();
        pthread_cond_signal(&notFull);
        printf("The consumer #%d has got resource.\n", ID);
        pthread_mutex_unlock(&reading);
        timeout();
    }
}

void timeout(){
    usleep(rand() % RAND_TIMEOUT + MIN_TIMEOUT);
}