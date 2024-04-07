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

// Task 1
// Givin two character strings s1 and s2 find
// the number of subs strings s1 that is the same as s2
void* getSubstring(void* arg){
        int threadId = *((int* ) arg)
}

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


// Producer thread that generates data nodes and add those
// data nodes to the global lsit
void * producer_thread( void *arg)
{
    bind_thread_to_cpu(*((int*)arg));//bind this thread to a CPU

    struct Node * ptr, tmp;
    int counter = 0;

    /* generate and attach K nodes to the global list */
    while( counter  < K )
    {
        ptr = generate_data_node();

        if( NULL != ptr )
        {
            while(1)
            {
                /* access the critical region and add a node to the global list */
                if( !pthread_mutex_trylock(&mutex_lock) )
                {
                    ptr->data  = 1;//generate data
                    /* attache the generated node to the global list */
                    if( List->header == NULL )
                    {
                        List->header = List->tail = ptr;
                    }
                    else
                    {
                        List->tail->next = ptr;
                        List->tail = ptr;
                    }
                    pthread_mutex_unlock(&mutex_lock);
                    break;
                }
            }
        }
        ++counter;
    }
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
        pthread_create(&(producer[i]), NULL, (void *) producer_thread, &cpu_array[i%NUM_PROCS]);
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
