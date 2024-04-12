#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX 1024
#define NUM_THREADS 4

int total = 0;
int n1, n2;
char *s1, *s2;
FILE *fp;
pthread_mutex_t mutex_total;

typedef struct {
    int start;
    int end;
} ThreadData;

void *substring_search(void *arg);

int main(int argc, char *argv[]) {
    pthread_t threads[NUM_THREADS];
    ThreadData data[NUM_THREADS];
    int segment_length;

    if ((fp = fopen("strings.txt", "r")) == NULL) {
        printf("ERROR: can't open strings.txt!\n");
        return 0;
    }
    s1 = (char *)malloc(sizeof(char) * MAX);
    s2 = (char *)malloc(sizeof(char) * MAX);
    s1 = fgets(s1, MAX, fp);
    s2 = fgets(s2, MAX, fp);
    n1 = strlen(s1) - 1;
    n2 = strlen(s2) - 1;
    fclose(fp);

    if (s1 == NULL || s2 == NULL || n1 < n2) {
        printf("Input error\n");
        return -1;
    }

    segment_length = n1 / NUM_THREADS;
    pthread_mutex_init(&mutex_total, NULL);

    for (int i = 0; i < NUM_THREADS; i++) {
        data[i].start = i * segment_length;
        data[i].end = (i == NUM_THREADS - 1) ? (n1 - n2) : ((i + 1) * segment_length - 1);
        pthread_create(&threads[i], NULL, substring_search, &data[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex_total);
    printf("The number of substrings is: %d\n", total);

    free(s1);
    free(s2);

    return 0;
}

void *substring_search(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int count = 0;
    for (int i = data->start; i <= data->end; i++) {
        int match = 1;
        for (int j = 0; j < n2 && (i+j) < n1; j++) {
            if (s1[i+j] != s2[j]) {
                match = 0;
                break;
            }
        }
        if (match) {
            count++;
        }
    }

    pthread_mutex_lock(&mutex_total);
    total += count;
    pthread_mutex_unlock(&mutex_total);

    pthread_exit(NULL);
}
