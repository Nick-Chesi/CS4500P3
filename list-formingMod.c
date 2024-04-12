/*
  list-forming.c:
  Each thread generates a data node, attaches it to a global list. This is reapeated for K times.
  There are num_threads threads. The value of "num_threads" is input by the student.

        Copy for the tasks, Nick Chesi Cole Bennett
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/param.h>
#include <sched.h>

#define K 200 // genreate a data node for K times in each thread

// Variables for task1
char* s1;
char* s2;
int globalCount = 0;
pthread_mutex_t countMutex;

// Nodes
struct Node
{
    int data;
    struct Node* next;
};

// Linked List
struct list
{
     struct Node * header;
     struct Node * tail;
};

// Mutex
pthread_mutex_t    mutex_lock;

// Pointer to the global list
struct list *List;

// Bind a thread to a cpu
void bind_thread_to_cpu(int cpuid) {
     cpu_set_t mask;
     CPU_ZERO(&mask);

     CPU_SET(cpuid, &mask);
     if (sched_setaffinity(0, sizeof(cpu_set_t), &mask)) {
         fprintf(stderr, "sched_setaffinity");
         exit(EXIT_FAILURE);
     }
}

// Dynamically generate new data node
struct Node* generate_data_node()
{
    struct Node *ptr;
    ptr = (struct Node *)malloc(sizeof(struct Node));

    if( NULL != ptr ){
        ptr->next = NULL;
    }
    else {
        printf("Node allocation failed!\n");
    }
    return ptr;
}

// Helper function to attach a local list to the
// global list
void attachLocalListToGlobal(struct list* localList) {
    if (localList->header == NULL) {
        return; // Local list is empty
    }
    pthread_mutex_lock(&mutex_lock);
    if (List->header == NULL) {
        // Global list is empty point to local list
                List->header = localList->header;
        List->tail = localList->tail;
    } else {
        // Attach local list to the end of the global list
        List->tail->next = localList->header;
        List->tail = localList->tail;
    }
    pthread_mutex_unlock(&mutex_lock);
}

// TASK 3 UPDATE
void* producerThread(void* arg){

        // bind cpu to a thread
        bind_thread_to_cpu(*((int*)arg));
        // local list for each thread
        struct list localList = {0};
        int counter = 0;

        // generate nodes and add them to list
        while(counter < K){
                struct Node* ptr = generate_data_node();
                // check if node is null then
                if(ptr != NULL){
                        // Check if first node is null, then
                        // if it is initalize it
                        if(localList.header == NULL){
                                // First node
                                localList.header = ptr;
                                localList.tail = ptr;
                        } else {
                                // Attach the new node to the end
                                localList.tail->next = ptr;
                                localList.tail = ptr;
                        }
                }
                // Increment counter to k nodes
                counter++;
        }
        // After generating local nodes attach to the global nodes
        attachLocalListToGlobal(&localList);
}

int main(int argc, char* argv[])
{
    int i, num_threads;

    int NUM_PROCS;//number of CPU
    int* cpu_array = NULL;

    struct Node  *tmp,*next;
    struct timeval starttime, endtime;

    if(argc == 1){
        printf("ERROR: please provide an input arg (the number of threads)\n");
        exit(1);
    }

    num_threads = atoi(argv[1]); //read num_threads from user
    pthread_t producer[num_threads];
    NUM_PROCS = sysconf(_SC_NPROCESSORS_CONF);//get number of CPU
    if( NUM_PROCS > 0)
    {
        cpu_array = (int *)malloc(NUM_PROCS*sizeof(int));
        if( cpu_array == NULL )
        {
            printf("Allocation failed!\n");
            exit(0);
        }
        else
        {
            for( i = 0; i < NUM_PROCS; i++)
               cpu_array[i] = i;
        }

    }

    pthread_mutex_init(&mutex_lock, NULL);


        // initializing the global list
    List = (struct list *)malloc(sizeof(struct list));
    if( NULL == List )
    {
       printf("End here\n");
       exit(0);
    }
    List->header = List->tail = NULL;

    gettimeofday(&starttime,NULL); //get program start time


        for( i = 0; i < num_threads; i++ )
    {
        pthread_create(&(producer[i]), NULL, (void *) producerThread, &cpu_array[i%NUM_PROCS]);
    }

    for( i = 0; i < num_threads; i++ )
    {
        if(producer[i] != 0)
        {
            pthread_join(producer[i],NULL);
        }
    }


    gettimeofday(&endtime,NULL); //get the finish time

        // free list nodes
    if( List->header != NULL )
    {
        next = tmp = List->header;
        while( tmp != NULL )
        {
           next = tmp->next;
           free(tmp);
           tmp = next;
        }
    }
    if( cpu_array!= NULL)
       free(cpu_array);

    /* calculate program runtime */
    printf("Total run time is %ld microseconds.\n", (endtime.tv_sec-starttime.tv_sec) * 1000000+(endtime.tv_usec-starttime.tv_usec));
    return 0;
}
