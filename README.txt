Resources:
https://man7.org/linux/man-pages/man2/sched_setaffinity.2.html
https://stackoverflow.com/questions/24034631/error-message-undefined-reference-for-cpu-zero

For compiling the first task:
have include -std-c99, it did not like my for loops?
gcc -o task1 task1.c -lpthread -std=c99

output from strings.txt:
=======================
abcdabsufsoababuosufba
ab
=======================
Substrings: 4

For the second task
gcc -o task2 task2.c -lpthread

output from message.txt:
=======================
hello i am a text file!
=======================
hello i am a text file!

DONE

For the third task
gcc list-forming.c -o mod-list-forming -pthread -D_GNU_SOURCE
gcc list-forming.c -o list-forming -pthread -D_GNU_SOURCE
I edited the producerThread to have the following code:

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


NOTE FOR TESTING PURPOSES THE EDITED LIST-FORMING.C IS NAMED list-formingMod.c, ALL TEST RESULTS FROM THE COMPILED BIN WILL SHOW THIS...
Note 1000 Microseconds = 1 Milisecond, 1000 Miliseconds = 1 second.
Test Results Supplied Code (K Values Tested: 200, 400, 800, 1600), (Threads testes 2, 4, 8, 16, 32, 64, 128):
K = 200:
2 Threads: Total run time is 602 microseconds.
4 Threads: Total run time is 2185 microseconds.
8 Threads: Total run time is 2991 microseconds.
16 Threads: Total run time is 5040 microseconds.
32 Threads: Total run time is 7116 microseconds.
64 Threads: Total run time is 14148 microseconds.
128 Threads: Total run time is 19270 microseconds.
K = 400:
2 Threads: Total run time is 759 microseconds.
4 Threads: Total run time is 1093 microseconds.
8 Threads: Total run time is 2199 microseconds.
16 Threads: Total run time is 4750 microseconds.
32 Threads: Total run time is 8806 microseconds.
64 Threads: Total run time is 23772 microseconds.
128 Threads: Total run time is 41674 microseconds.
K = 800
2 Threads: Total run time is 1108 microseconds.
4 Threads: Total run time is 2786 microseconds.
8 Threads: Total run time is 4545 microseconds.
16 Threads: Total run time is 7321 microseconds.
32 Threads: Total run time is 19445 microseconds.
64 Threads: Total run time is 41545 microseconds.
128 Threads: Total run time is 75146 microseconds.
K = 1600
2 Threads: 
4 Threads: 
8 Threads: 
16 Threads: 
32 Threads: 
64 Threads: 
128 Threads: 

Test Results Modified Code (K Values Tested: 200, 400, 800, 1600), (Threads testes 2, 4, 8, 16, 32, 64, 128):
K = 200
2 Threads: 
4 Threads: 
8 Threads: 
16 Threads: 
32 Threads: 
64 Threads: 
128 Threads: 
K = 400
2 Threads: 
4 Threads: 
8 Threads: 
16 Threads: 
32 Threads: 
64 Threads: 
128 Threads: 
K = 800
2 Threads: 
4 Threads: 
8 Threads: 
16 Threads: 
32 Threads: 
64 Threads: 
128 Threads: 
K = 1600
2 Threads: 
4 Threads: 
8 Threads: 
16 Threads: 
32 Threads: 
64 Threads: 
128 Threads: 
