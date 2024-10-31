// Implementation of Consumer Process in the Producer/Consumer Problem.
// Parker Franklin

// Large portions of the code in "consumer.cpp" were created based on my own
// personal research of the Producer/Consumer Problem from both online and
// in-class sources. Some of the notable resources I used to help put this 
// file together include:
//     - "What is Shared Memory in Linux?" by Vasu Bansal of Scaler
//     - "How to use POSIX semaphores in C language" by GeeksForGeeks
//     - "Producer-Consumer Problem and its Implementation with C++" by TutorialsPoint
//     - Chapter 3 Slides from Operating Systems
//     - Any additional Article/Forum/Code references found in class or online
//
// Every resource that helped in creating this file was used purely for
// my own reference while working on Assignment 1!

#include "buffer.hpp"

int main() {

    //
    // Default scenario of creating a Shared Memory Pointer and testing various conditions
    // to assure it is linked properly for use by the Producer & Consumer Processes.
    //

    sleep(1); // Convention used to make sure that the Producer Process gets ahead of the Consumer Process
              // on startup. (This is just something that I read about and do not know if it is necessary!)

    const char* shmPtr = "/sharedMemObject"; // Creates a Shared Memory Pointer and links it to string.
    int fileDescriptor = shm_open(shmPtr, O_RDWR, S_IRUSR | S_IWUSR); // Creates a File Descriptor that opens/creates
                                                                      // the Shared Memory Object with a parameter
                                                                      // for its creation (O_RDWR) as well as
                                                                      // its access (S_IRUSR | S_IWUSR).

    buffer* shmp = (struct buffer*)mmap(NULL, sizeof(*shmp), PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0); // Casts the 'mmap()' function to the type of
                                                                                                                     // 'struct buffer*' to assign the Shared Memory
                                                                                                                     // Object to the memory space of the producer.
    
    //
    // Main conditional loop that allows the Consumer to consume items and remove them from the buffer.
    //

    for (int i = 0; i < 6; ++i) { //limiting to 6 times to avoid too much output
        sem_wait(&shmp->semFull);
        sem_wait(&shmp->S);

        // Decrements the global counter varaible of the Producer & Consumer and then stores
        // the value of the consumed item after it has been removed.
        shmp->count--;
        int consumedItem = shmp->data[shmp->count];

        // Prints out all items that are currently in the buffer.
        std::cout << "The items currently in the buffer are: ";
        for (int j = 0; j < 2; ++j) {
            std::cout << shmp->data[j] << " ";
        }

        // Resets the global counter variable and displays information on what has been consumed by the Consumer.
        shmp->data[shmp->count] = 0;
        std::cout << '\n' << "The Consumer has consumed: " << consumedItem << std::endl;

        // Process Synchronization from the Consumer to the Prodcer using shared variables.
        sem_post(&shmp->semEmpty); // Signals that the buffer had 1 item removed from it.
        sem_post(&shmp->S); // Signals to the Producer that the Consumer has finished executing.
    }

    shm_unlink(shmPtr);
    exit(EXIT_SUCCESS);
}