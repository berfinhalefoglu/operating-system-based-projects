# operating-system-based-projects
# Operating Systems Synchronization Problems

This project implements classical operating system synchronization problems using C and POSIX threads.

## Implemented Problems
- Dining Philosophers Problem
- Sleeping Barber Problem

## Features
- Multithreaded implementation
- Mutex and semaphore based synchronization
- Deadlock and starvation prevention
- Console-based simulation output

## Technologies
- C
- POSIX Threads (pthreads)
- GCC Compiler
- Linux / Unix environment

## Files
- dining_philosophers.c  
- sleeping_barber.c  
- test_script.sh  
- output_philosophers_*.txt  
- output_barber.txt  

## How to Compile and Run

Compile:
gcc dining_philosophers.c -lpthread -o dining_philosophers
gcc sleeping_barber.c -lpthread -o sleeping_barber


Run:
./dining_philosophers
./sleeping_barber


## What I Learned
- Thread creation and management
- Process synchronization
- Mutex and semaphore usage
- Handling concurrency issues

## Note
This project was developed for educational purposes as part of an Operating Systems course.
