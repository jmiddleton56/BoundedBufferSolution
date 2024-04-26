/* Created by Joel Middleton
    April 5th, 2024 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>

#define BUFFER_SIZE 5
typedef int buffer_item;
buffer_item buffer[BUFFER_SIZE];
sem_t full, empty;
pthread_mutex_t mutex;
bool running = true;
unsigned char insertIndex = 0;
unsigned char removeIndex = 0;
unsigned char thread_ID[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}; // Thread ID's

void *producer(void *arg);
void *consumer(void *arg);
void tempTest(int threadID);
void insert_item(buffer_item item, int *threadID);
void remove_item(int *threadID);

int main(int argc, char *argv[]) {
    srand((unsigned int) time(NULL)); // Use current time as seed for random generator
    // 1. Get command-line arguments argv[1],argv[2],argv[3]
    unsigned char sleepTime = atoi(argv[1]); // Changed from ints to unsigned chars to avoid warning, still unsure of why this was happening
    sleepTime += 20;
    unsigned char num_producers = atoi(argv[2]);
    unsigned char num_consumers = atoi(argv[3]);

    // 2. Initialize semaphores and mutex lock
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFFER_SIZE);
    pthread_mutex_init(&mutex, NULL);

    // 3. Initialize buffer
    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer[i] = -1;
    }

    // 4. Create producer thread(s)
    pthread_t producer_Threads[num_producers]; // Create array of producer threads
    for (int i = 0; i < num_producers; i++) {
        int *prod_ID = malloc(sizeof(int));
        *prod_ID = thread_ID[i];
        pthread_create(&producer_Threads[i], NULL, producer, prod_ID); // Create producer thread
    }

    // 5. Create consumer thread(s)
    pthread_t consumerThreads[num_consumers]; // Create array of consumer threads
    for (int i = 0; i < num_consumers; i++) {
        int *prod_ID = malloc(sizeof(int));
        *prod_ID = thread_ID[i];
        pthread_create(&consumerThreads[i], NULL, consumer, prod_ID); // Create producer thread
    }

    // 6. Sleep and thereafter terminate the C program
    sleep(sleepTime);
    running = false;
    sem_destroy(&full);
    sem_destroy(&empty);
    pthread_mutex_destroy(&mutex);
    return 0;
}

void *producer(void *arg) {
    buffer_item item;
    int *threadID = (int *) arg; // Cast arg to int pointer which is the thread ID
    while (running) {
        // Sleep for a random period of time: 0-4 seconds
        int rand_Sleep = rand() % 5;
        sleep(rand_Sleep); 
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        // Generate a random number 
        item = rand(); 
        // Insert an item 
        insert_item(item, threadID);
        sem_post(&full); 
        pthread_mutex_unlock(&mutex);
    }
    pthread_mutex_unlock(&mutex);
    pthread_exit(0);
}

void *consumer(void *arg) {
    int *threadID = (int *) arg; // Cast arg to int pointer which is the thread ID
    while (running) {
        // Sleep for a random period of time: 0-4 seconds
        int rand_Sleep = rand() % 5;
        sleep(rand_Sleep);
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        // Remove an item 
        remove_item(threadID);
        sem_post(&empty);
        pthread_mutex_unlock(&mutex);
    }
    pthread_mutex_unlock(&mutex);
    pthread_exit(0);
}

void insert_item(buffer_item item, int *threadID) {
    buffer[insertIndex] = item; // Insert item into buffer
    printf("Producer %d inserted item %i into buffer[%d]\n", *threadID, item, insertIndex);
    if (insertIndex == BUFFER_SIZE - 1) { // If insertIndex is at the end of the buffer, reset to 0
        insertIndex = 0;
    } else {
        insertIndex++; // Otherwise increment insertIndex
    }
}

void remove_item(int *threadID) {
    buffer_item item = buffer[removeIndex]; // Copy item from buffer for print statement
    buffer[removeIndex] = -1; // Remove item from buffer
    printf("Consumer %d removed item %i from buffer[%d]\n", *threadID, item, removeIndex);
    if (removeIndex == BUFFER_SIZE - 1) { // If removeIndex is at the end of the buffer, reset to 0
        removeIndex = 0;
    } else {
        removeIndex++; // Otherwise increment removeIndex
    }
}