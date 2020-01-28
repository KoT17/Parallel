Primes (Part 1 of Homework 1)
-----------------------------

I used C++ as my language of choice using std=c++11. I coded the project on a folder in Windows 10, using Atom as my IDE.

When running my program, the following command in a linux/ubuntu ternimal.

g++ -pthread prime.cpp

Followed by "./a.out", to run the given code

(-pthread) is a required flag to ensure that the code is run using concurrency instead of on one main thread

Dependencies
------------
thread - needed to create threads in the environment

iostream - mainly used for error handling and logging

fstream - used to write results to file

math.h - math calculations for using the sieve 

vector - utilized across the program for array functionality

chrono - used to determine speed at which the threads completed in

Approach
--------

The main approach of this problem was the use of 8 threads to divide the work evenly and optimize it with a sieve. I utilize the sieve of eratosthenes to determine prime numbers without needing to loop through n=2 to sqrt(n).

Each thread was given an array of numbers and a pointer to the sieve, which all threads shared. Since the threads are all writing one way, this allows for there to be no conflicts for writing and no deadlocks.

Previous approaches used atomic counters to determine different states but were moved out due to the current approach.

Once all the computation from the threads have been complete, there are a series of for loops to determine different information for printing out to "prime.txt"

Error handling
--------------
It is possible that you may need to include the flag -std=c++11 where running the compile command. I had this problem on my laptop.
