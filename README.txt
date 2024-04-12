Resources:
https://man7.org/linux/man-pages/man2/sched_setaffinity.2.html

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
