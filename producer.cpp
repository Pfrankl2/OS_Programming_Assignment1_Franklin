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
#include <pthread.h>

void* producer(void* arg) {
    buffer* shmp = static_cast<buffer*>(arg);

    for (int i = 0; i < 6; ++i) {
        sem_wait(&shmp->semEmpty); // Waits for an empty slot.
        sem_wait(&shmp->S); // Wait for exclusive access to the buffer.

        int itemValue = 1 + i; // Creates a new item.
        shmp->data[shmp->count] = itemValue; // Stores the new item in the buffer.
        std::cout << "The Producer has produced: " << shmp->data[shmp->count] << std::endl;
        shmp->count++;

        sem_post(&shmp->S); // Releases exclusive access of the buffer.
        sem_post(&shmp->semFull); // Signals that a new item is available.
    }
    return nullptr;
}

int main() {
    const char* shmPtr = "/sharedMemObject";
    shm_unlink(shmPtr);
    int fileDescriptor = shm_open(shmPtr, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);

    ftruncate(fileDescriptor, sizeof(buffer));
    buffer* shmp = static_cast<buffer*>(mmap(nullptr, sizeof(buffer), PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0));

    sem_init(&shmp->semFull, 1, 0); // Buffer is initially empty.
    sem_init(&shmp->semEmpty, 1, 2);
    sem_init(&shmp->S, 1, 1); // Semaphore used for mutual exclusion.

    pthread_t producerThread;
    pthread_create(&producerThread, nullptr, producer, shmp);

    // Waits for producer thread to finish executing.
    pthread_join(producerThread, nullptr);

    return EXIT_SUCCESS;
}
