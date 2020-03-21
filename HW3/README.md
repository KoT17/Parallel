RDCSS
---------------

I used C++ as my language of choice using std=c++11. I coded the project on a folder in Windows 10, using Atom as my IDE.

When running my program, the following command in a linux/ubuntu ternimal.

g++ -pthread RDCSS.cpp

Followed by "./a.out", to run the given code

The result shows the result of the atomic counter which represents the number of operations

(-pthread) is a required flag to ensure that the code is run using concurrency instead of on one main thread

You may need to include the flag of -std=c++11 depending on what environment you are running this on

Dependencies
------------

thread - needed to create threads in the environment

iostream - mainly used for error handling and results

cstdint - used to have access to uintptr_t

chrono - used to determine flag timer at which the threads will be told to complete

atomic - atomic variables to ensure lock-free

vector - used for the initilization of threads in main


Implementation
--------------

My approach followed the implementation provided by the research paper of Tim Harris (https://timharris.uk/papers/2002-disc.pdf). 

This paper provides the fundamental structure of the RDCSS with the psuedo code in section 4.1. Where it goes on to explain the role of a descriptor object which will keep track of our addresses in the RDCSS.


I decided to use uintptr_t from the cstdint library to represent word_t since the pointer can represent multiple address and can manipulate different address with casting and reinterpret.

For the descriptor struct and the atomic flags, in the RDCSS there are only two addresses that can be modified across the threads which is the control and data address. With the other values being the expected values, which are used in the CAS1. These uintptr_t must be placed as atomic so they can properly be loaded inside of the respective function.

The uint_ptr used by the controlTimer() thread as well as the counter for the amount of operations, must be atomic to allow for all threads to safely read and modify the values without ruining other threads.

When creating threads, each thread must have a unique descriptor with new addresses to the flag and counter pointers. These pointers will be modified to represent different "values" to show whether the address is a descriptor or not. This is done by bit-marking the address by appending a 1 to the end of the address and then removing it. This allows for a simple check on the uintptr_t inside of the isDescriptor function.

For determining n2 as well as storing the atomic flag/counter, it's important to dereference and cast the uint_ptr so that the internal operations of the struct can be proper done. Such as comparing if the flag is true or not as well as retrieving the unsigned long of the counter.

Graphs
------

I have created a subfolder which contains the graphs that show the throughput of the RDCSS compared with the amount of threads. 

Slowdown (Response to Question in Assignment)
---------------------------------------------

Due to the increasing number of threads that are utilizing the atomic values inside of a descriptor or the counter uintptr. There is more contention for the variables, meaning that the threads must wait longer to complete the operations. Increasing the number of threads past the point of 2 threads seems to cause a slowdown in performance. 
