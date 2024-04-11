#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Assuming a sensible default for maximum threads.
#define MAX_THREADS 8
// Mutex for updating the global match count.
pthread_mutex_t lock;

char *s1;
char *s2;
int globalMatchCount = 0;

typedef struct {
    int start_offset;
} ThreadArgs;

void* substring_search(void* args);

int main() {
    pthread_t threads[MAX_THREADS];
    ThreadArgs args[MAX_THREADS];
    FILE *file;
    char *filename = "string.txt";
    size_t len = 0;
    ssize_t read;

    file = fopen(filename, "r");
    if (file == NULL) {
        exit(EXIT_FAILURE);
    }

    s1 = (char *)malloc(1000 * sizeof(char));
    s2 = (char *)malloc(1000 * sizeof(char));

    // Read s1
    if ((read = getline(&s1, &len, file)) != -1) {
        s1[strcspn(s1, "\n")] = 0; // Removing newline character
    }

    // Read s2
    if ((read = getline(&s2, &len, file)) != -1) {
        s2[strcspn(s2, "\n")] = 0; // Removing newline character
    }

    fclose(file);

    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\n mutex init has failed\n");
        return 1;
    }

    for (int i = 0; i < MAX_THREADS; i++) {
        args[i].start_offset = i;
        if (pthread_create(&threads[i], NULL, substring_search, (void*)&args[i]) != 0) {
            printf("Failed to create thread\n");
        }
    }

    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Total matches: %d\n", globalMatchCount);

    pthread_mutex_destroy(&lock);

    free(s1);
    free(s2);

    return 0;
}

void* substring_search(void* args) {
    ThreadArgs *threadArgs = (ThreadArgs*)args;
    int offset = threadArgs->start_offset;
    int len_s1 = strlen(s1);
    int len_s2 = strlen(s2);
    int count = 0;

    for (int i = offset; i <= len_s1 - len_s2; i += MAX_THREADS) {
        if (strncmp(s1 + i, s2, len_s2) == 0) {
            count++;
        }
    }

    // Lock before updating the global counter and unlock afterward
    pthread_mutex_lock(&lock);
    globalMatchCount += count;
    pthread_mutex_unlock(&lock);

    return NULL;
}
