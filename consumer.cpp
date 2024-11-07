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
#include <pthread.h>

void* consumer(void* arg) {
    buffer* shmp = static_cast<buffer*>(arg);

    // Main logical loop of Consumer Implementation for "i < #" items.
    for (int i = 0; i < 6; ++i) {
        sem_wait(&shmp->semFull); // Wait until there's an item to consume.
        sem_wait(&shmp->S); // Wait for exclusive access to the buffer.

        if (shmp->count > 0) {
            std::cout << "The items currently in the buffer are: ";
            for (int j = 0; j < shmp->count; ++j) {
                std::cout << shmp->data[j] << " ";
            }

            // Consume the oldest item in the buffer.
            int consumedItem = shmp->data[0];

            // Shift items in the buffer after consumption.
            for (int j = 1; j < shmp->count; ++j) {
                shmp->data[j - 1] = shmp->data[j];
            }

            // Clears the last slot of the buffer and updates 'count'.
            shmp->data[shmp->count - 1] = 0;
            shmp->count--;

            std::cout << "\nThe Consumer has consumed: " << consumedItem << std::endl;
        }

        sem_post(&shmp->S); // Release exclusive access from consumer.
        sem_post(&shmp->semEmpty); // Signals that an item has been removed from the buffer.
    }
    return nullptr;
}

int main() {
    const char* shmPtr = "/sharedMemObject";
    int fileDescriptor = shm_open(shmPtr, O_RDWR, S_IRUSR | S_IWUSR);
    buffer* shmp = static_cast<buffer*>(mmap(nullptr, sizeof(buffer), PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0));

    pthread_t consumerThread;
    pthread_create(&consumerThread, nullptr, consumer, shmp);

    // Waits for the consumer thread to finish executing.
    pthread_join(consumerThread, nullptr);

    shm_unlink(shmPtr);
    return EXIT_SUCCESS;
}
