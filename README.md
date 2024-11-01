# Overview
The goal of Assignment 1 was to simulate the Producer-Consumer Problem seen in process synchronization. The requirments for this assignment were to use three files ('buffer.hpp' / 'producer.cpp' / 'consumer.cpp') in a Linux environment to demonstrate the Consumer-Producer Problem with a bounded buffer of size 2, Linux's shared memory and semaphores.

The header file 'structure.hpp' holds all of the include files needed to access critical functionality of this problem, such as that of semaphores and shared memory (when run in a Linux environment). Additionally, the 'buffer' structure is defined in this file to be used as a representation of a real system buffer that can hold various data items. For my simulation I used an array of integers to act as each item in the buffer, where a 0 represents an empty space in the buffer.

The other two files 'producer.cpp' and 'consumer.cpp' each implement the necessary code to create a representation of a Producer and a Consumer process, respectively. A buffer object pointer 'buffer* shmp' is used to create and manage the buffer of items as they are placed into and removed from shared memory by the two processes. The main functionality of both processes is done within a for-loop, where the Producer first signals using semaphores its intent to produce an item and then the Consumer (If there is an item available after waiting on the Producer) consumes said item. The Producer will always execute first and mutual exclusion is respected between the two processes.

An example output for this program can be seen in the text file 'exampleOne.txt' in this repository!

# Running The Code
To compile both 'producer.cpp' and 'consumer.cpp' the following Linux commands are used:

        g++ producer.cpp -pthread -lrt -o producer
        g++ consumer.cpp -pthread -lrt -o consumer

After creating these two object files, the program can be run with the following command:

        ./producer & ./consumer
