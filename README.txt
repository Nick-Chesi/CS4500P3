Username: root
Password: 1234!@#$

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

Description of changes:
I mainly reduced the usage of pthread_mutex_trylock, which is a busy wait loop so that we can decrease cpu usage
because we will not be in a busy wait cycle. I also modified the code to reduce the usage of locks by attempting to 
reduce the number of times a thread interacts with a locked resource.

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
Note 1000 Microseconds = 1 Milisecond, 1000 Miliseconds = 1 second. Also as K value increased the time increased as well, same with threads.
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
2 Threads: Total run time is 1972 microseconds.
4 Threads: Total run time is 5846 microseconds.
8 Threads: Total run time is 9515 microseconds.
16 Threads: Total run time is 21615 microseconds.
32 Threads: Total run time is 32084 microseconds.
64 Threads: Total run time is 74302 microseconds.
128 Threads: Total run time is 148132 microseconds.

Test Results Modified Code (K Values Tested: 200, 400, 800, 1600), (Threads testes 2, 4, 8, 16, 32, 64, 128):
K = 200
2 Threads: Total run time is 454 microseconds.
4 Threads: Total run time is 582 microseconds.
8 Threads: Total run time is 2511 microseconds.
16 Threads: Total run time is 3601 microseconds.
32 Threads: Total run time is 16837 microseconds.
64 Threads: Total run time is 10714 microseconds.
128 Threads: Total run time is 15958 microseconds.
K = 400
2 Threads: Total run time is 451 microseconds.
4 Threads: Total run time is 522 microseconds.
8 Threads: Total run time is 2339 microseconds.
16 Threads: Total run time is 4064 microseconds.
32 Threads: Total run time is 8720 microseconds.
64 Threads: Total run time is 13359 microseconds.
128 Threads: Total run time is 20081 microseconds.
K = 800
2 Threads:  Total run time is 487 microseconds.
4 Threads: Total run time is 718 microseconds.
8 Threads: Total run time is 2534 microseconds.
16 Threads: Total run time is 7673 microseconds.
32 Threads: Total run time is 8557 microseconds.
64 Threads: Total run time is 22141 microseconds.
128 Threads: Total run time is 37681 microseconds.
K = 1600
2 Threads: Total run time is 587 microseconds.
4 Threads: Total run time is 1945 microseconds.
8 Threads: Total run time is 1988 microseconds.
16 Threads: Total run time is 5486 microseconds.
32 Threads: Total run time is 13274 microseconds.
64 Threads: Total run time is 25819 microseconds.
128 Threads: Total run time is 56783 microseconds.
4096: Threads: Total run time is 1827336 microseconds.
