#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// max len for string
// and number of threads
#define MAX 1024
#define NUM_THREADS 4


// global vars
// total sun strings
// lengths of s1 and s2
// file pointer
// and mutex for sync
int total = 0;
int n1, n2;
char *s1, *s2;
FILE *fp;
pthread_mutex_t mutex_total;

// struct to hold each threads data
typedef struct {
    int start;
    int end;
} ThreadData;

// function def
void *substring_search(void *arg);

int main(int argc, char *argv[]) {
        // array of each identifyer
        pthread_t threads[NUM_THREADS];
    ThreadData data[NUM_THREADS];
        // segment length
    int segment_length;

        // open file
        if ((fp = fopen("strings.txt", "r")) == NULL) {
            printf("CANNOT OPEN strings.txt!\n");
            return 0;
        }
        // s1 and s2 size reservationb
        s1 = (char *)malloc(sizeof(char) * MAX);
        s2 = (char *)malloc(sizeof(char) * MAX);
        // setting s and s2 to the respective chars*
        s1 = fgets(s1, MAX, fp);
        s2 = fgets(s2, MAX, fp);
        // getting string length for each
        n1 = strlen(s1) - 1;
        n2 = strlen(s2) - 1;
        // close file
        fclose(fp);

        // input validation
        if (s1 == NULL || s2 == NULL || n1 < n2) {
            printf("Input error\n");
            return -1;
        }
            // gets length of segment
        segment_length = n1 / NUM_THREADS;
        // initialize mutex
        pthread_mutex_init(&mutex_total, NULL);

        // create threads
        for (int i = 0; i < NUM_THREADS; i++) {
            data[i].start = i * segment_length;
            data[i].end = (i == NUM_THREADS - 1) ? (n1 - n2) : ((i + 1) * segment_length - 1);
            pthread_create(&threads[i], NULL, substring_search, &data[i]);
        }
        // wait for threads to finish
        for (int i = 0; i < NUM_THREADS; i++) {
            pthread_join(threads[i], NULL);
        }
        // destroy and print result
        pthread_mutex_destroy(&mutex_total);
        printf("Substrings: %d\n", total);
        // free memory
        free(s1);
        free(s2);

        return 0;
}

void *substring_search(void *arg) {
        // thread data pointer
    ThreadData *data = (ThreadData *)arg;
    int count = 0; // count for each thread
    for (int i = data->start; i <= data->end; i++) {
        int match = 1; // flag for substring match
        for (int j = 0; j < n2 && (i+j) < n1; j++) {
            if (s1[i+j] != s2[j]) {
                                // if found reset to zero and break out
                match = 0;
                break;
            }
        }
                // increment local count if match
            if (match) {
                count++;
            }
    }
        // lock while counting, global total
        pthread_mutex_lock(&mutex_total);
        total += count;
        // unlock after counting
        pthread_mutex_unlock(&mutex_total);

        pthread_exit(NULL);
}
