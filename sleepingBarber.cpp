#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <queue>
using namespace std;

#define N_CHAIR 5
#define N_CLIENTS 100

void* Bradobrey(void* arg);
void* Client(void* arg);

typedef struct arguments {
    int n;
} ARG;

pthread_mutex_t  barberSleep,chairLock, barberWork;

queue<int> chairs;
int main(int argc, const char *argv[]) {
    ARG arg[N_CLIENTS];
    pthread_t barber;
    pthread_t clients[N_CLIENTS];
    pthread_mutex_init(&barberSleep, NULL);
    pthread_mutex_init(&chairLock, NULL);
    pthread_mutex_lock(&barberSleep);
    pthread_mutex_lock(&barberWork);
    pthread_create(&barber, NULL, Bradobrey, (void*) &arg);
    for(int i =0; i < N_CLIENTS; ++i) {
        arg[i].n = i;
        pthread_create(&(clients[i]), NULL, Client, (void*) &arg[i]);
        usleep(rand() % 6000000 + 200000);
    }
    pthread_join(barber, NULL);
    return 0;
}

void* Bradobrey(void* arg) {
    bool t = true;
    int n;
    while (t) {
        if (pthread_mutex_trylock(&barberSleep) != 0) {
            cout << "Barber sleep\n";
            pthread_mutex_lock(&barberSleep);
        }
        pthread_mutex_lock(&chairLock);
        while (chairs.size()>0) {
            n = chairs.front();
            chairs.pop();
            pthread_mutex_unlock(&chairLock);
            printf("Barber starts cutting hair of customer %d\n",n);
            usleep(rand() % 1000000 + 500000);
            printf("Barber end cutting hair of customer %d\n",n);
            pthread_mutex_lock(&chairLock);
            pthread_mutex_unlock(&barberWork);
            usleep(rand() % 1000 + 5000);
        }
        pthread_mutex_unlock(&chairLock);
        pthread_mutex_trylock(&barberWork);
    }
    return 0;
}

void* Client(void* arg) {
    ARG *temp = (ARG*) arg;
    pthread_mutex_lock(&chairLock);
    if(chairs.size()  <= N_CHAIR) {
        if(chairs.size() == 0)
            pthread_mutex_unlock(&barberSleep);
        printf("Customer %d has entered a barber shop.\n", temp->n);
        chairs.push(temp->n);
        pthread_mutex_unlock(&chairLock);
        pthread_mutex_lock(&barberWork);
        printf("Customer %d exit\n", temp->n);
    } else {
        printf("Client %d NOT entered a barber shop.\n", temp->n);
    }
    pthread_mutex_unlock(&chairLock);
}
