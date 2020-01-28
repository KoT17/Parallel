Philosophers (Part 2 of Homework 1)
-----------------------------

I used C++ as my language of choice using std=c++11. I coded the project on a folder in Windows 10, using Atom as my IDE.

When running my program, the following command in a linux/ubuntu ternimal.

g++ -pthread Dining(A/B/C/D).cpp Chopstick.cpp Philosopher.cpp

Followed by "./a.out", to run the given code. For Version 4, enter the amount of threads or "Philosophers" you want to produce

To ternimate the program, enter "n" and press enter. The parentheses next to the Dining.cpp file is to change which version of the problem, corresponding to (1/2/3/4)

(-pthread) is a required flag to ensure that the code is run using concurrency instead of on one main thread

Dependencies
------------
thread - needed to create threads in the environment

iostream - mainly used for error handling and logging

mutex - used across the classes to allow for threads to lock objects (Ex: Chopstick)

vector - utilized across the program for array functionality

chrono - used to determine speed at which the threads completed in

cstring - used for exit functionality for comparing entered value to ternimate program

atomic - used for setting flag event when a key is pressed

queue (Version 3/4) - used for solving stravation problem where not all philosophers get a turn


Approach
--------

The initial approach for Version 1 was to create two classes to represent the chopsticks on the table and the philsophers who are sitting down.

At the table, there is an array for all the chopsticks and each philosopher is given two fields that represent the chopstick to their right and left

When a philosopher goes to grab a chopstick, they all grab their left one first. This can result in a deadlock if all threads lock their left chopstick because the other philosophers will not be able to access the right one.

This problem is solved in Version 2, where the lock on the mutexs of the Chopsticks are locked on the same lines of code. Ensuring that both chopsticks have to be unlocked for them to be used by the philosopher.


In Version 3 and 4, I use a queue to order the philosophers by the time they complete their thread. Once a thread is finished with eating, they are moved to the back of the queue to allow fore other threads that haven't had a chance to eat.

Specifically in part 4, the user inputs a variable to determine the amount of threads to run the program with. The queue solution does not change as it allows for all philosophers to finish eating before another thread can grab the chopsticks.



Error handling
--------------
It is possible that you may need to include the flag -std=c++11 where running the compile command. I had this problem on my laptop.
