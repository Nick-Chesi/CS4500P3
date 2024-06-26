#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// circular queue buffersize
#define BUFFER_SIZE 12

// declaring the buffer
// queue
char buffer[BUFFER_SIZE];
int add = 0; // index for add
int rem = 0; // index for rm / remove
int num = 0; // elements buffer

// consumer flag finished
int finished = 0;

// Mutex for protecting access to the buffer
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
// Condition var for signaling for producer consumer
pthread_cond_t c_consumer = PTHREAD_COND_INITIALIZER;
pthread_cond_t c_producer = PTHREAD_COND_INITIALIZER;

// delarations
void insert(char item);
char rm();
void *consumer(void *param);
void *producer(void *param);


int main(int argc, char *argv[]) {
        // initalizing the p threads
    pthread_t prod, cons;
        // creates the producer and consumer threads
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);
        // wait for producer adn consumer thread
        // to finish
    pthread_join(prod, NULL);
        pthread_join(cons, NULL);

    printf("\nDONE\n");
    return 0;
}

// function to insert items into the circular buffer
void insert(char item) {
    buffer[add] = item;
    add = (add + 1) % BUFFER_SIZE;
    num++;
}

// remove from circular buffer
char rm() {
    char item = buffer[rem];
    rem = (rem + 1) % BUFFER_SIZE;
    num--;
    return item;
}

// producer thread
void *producer(void *param) {
        // open file
    FILE *fp = fopen("message.txt", "r");
    if (fp == NULL) {
        perror("Unable to open the file");
        exit(1);
    }

        // read until EOF
        // lock the mutex before changing the buffer
        // wait if the buffer is full
           char item;
    while ((item = fgetc(fp)) != EOF) {
        pthread_mutex_lock(&m);
        while (num == BUFFER_SIZE) {
            pthread_cond_wait(&c_producer, &m);
        }

        insert(item);
        pthread_cond_signal(&c_consumer);
        pthread_mutex_unlock(&m);
    }

    // Insert a sentinel value to indicate end-of-data
    pthread_mutex_lock(&m);
    while (num == BUFFER_SIZE) {
        pthread_cond_wait(&c_producer, &m);
    }
    insert('\0'); // Sentinel value for end-of-data
    pthread_cond_signal(&c_consumer);
    pthread_mutex_unlock(&m);

    fclose(fp);
}

// Consumer thread

void *consumer(void *param) {
    char item;

        // loop until until all items are
        // removed from the buffer

    while (1) {
                // Lock the mutex before buffer access
                // Wait if buffer is empty
         while (num == 0) {
            pthread_cond_wait(&c_consumer, &m);
        }

        item = rm();
        if (item == '\0') { // Check for the sentinel value
            pthread_mutex_unlock(&m);
            break; // Exit the loop and end the thread
        }
        printf("%c", item);
        fflush(stdout);
                // signal producer done
                // unlock mutex
        pthread_cond_signal(&c_producer);
        pthread_mutex_unlock(&m);
    }
}
