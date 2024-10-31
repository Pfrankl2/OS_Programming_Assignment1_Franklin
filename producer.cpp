// Implementation of Producer Process in the Producer/Consumer Problem.
// Parker Franklin

// Large portions of the code in "producer.cpp" were created based on my own
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

    const char* shmPtr = "/sharedMemObject"; // Creates a Shared Memory Pointer and links it to string.
    shm_unlink(shmPtr); // Unlinks 'shmPtr' for testing so that no other memory locations of the same name exist.

    int fileDescriptor = shm_open(shmPtr, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR); // Creates a File Descriptor that opens/creates
                                                                                         // the Shared Memory Object with some parameters for
                                                                                         // for its creation (O_CREAT | O_EXCL | O_RDWR) as well as
                                                                                         // for its access (S_IRUSR | S_IWUSR).
    
    // Resizing of the Shared Memory Object to the size of 'struct buffer'.
    if (ftruncate(fileDescriptor, sizeof(struct buffer)) == -1) {
        std::cout << "Process 'Producer' returned with 'ftruncate == -1'." << std::endl;
        exit(0);
    }

    buffer* shmp = (struct buffer*)mmap(NULL, sizeof(*shmp), PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0); // Casts the 'mmap()' function to the type of
                                                                                                                     // 'struct buffer*' to assign the Shared Memory
                                                                                                                     // Object to the memory space of the producer.

    // Initializes the semaphores of the Producer/Consumer Processes with Error Handling.
    if (sem_init(&shmp->semFull, 1, 0) == -1) { // If there exist no full slots of the buffer:
        std::cout << "Producer: 'semFull' == -1." << std::endl;
        exit(0);
    }
    if (sem_init(&shmp->semEmpty, 1, 2) == -1) { // If there exist 2 (All) empty slots of the buffer:
        std::cout << "Producer: 'semEmpty' == -1." << std::endl;
        exit(0);
    }
    if (sem_init(&shmp->S, 1, 1) == -1) { // If the semaphore to the lock returns false:
        std::cout << "Producer: S == -1" << std::endl;
        exit(0);
    }

    //
    // Main conditional loop that allows the Producer to create items and stores them in the buffer.
    //

    for (int i = 0; i < 6; ++i) { // For creating 'i < 6' new items:
        sem_wait(&shmp->semEmpty);
        sem_wait(&shmp->S);

        int itemValue = 1 + i; // Sets the value of the newly-produced item.

        // Stores the item produced into the buffer using the pointer 'shmp'.
        shmp->data[shmp->count] = itemValue;
        std::cout << "The Producer has produced: " << shmp->data[shmp->count] << std::endl;
        shmp->count++; // Increments the shared 'count' variable to go to the next element in the buffer of pointer 'shmp'.

        // Process Synchronization from the Producer to the Consumer using shared variables.
        sem_post(&shmp->S); // Signals to the Consumer that the Producer has finished executing.
        sem_post(&shmp->semFull); // Signals that the buffer has been filled with 1 more item.

    }

    exit(EXIT_SUCCESS);
}