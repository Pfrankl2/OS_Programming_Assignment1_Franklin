// Defines a structure for the buffer of the Producer/Consumer Problem.
// Parker Franklin

#ifndef BUFFER_HPP
#define BUFFER_HPP

// Large portions of the code in "buffer.hpp" were created based on my own
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

#include <iostream>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>

// Definition of Buffer Structure
struct buffer {
    int data[2]; // Array of 'int' to hold data.
    int count = 0; // Counter used to keep track of the semaphore location.
    sem_t semFull;
    sem_t semEmpty;
    sem_t S; // Three semaphores that allow the Producer and Consumer to communicate buffer state.
};

#endif
