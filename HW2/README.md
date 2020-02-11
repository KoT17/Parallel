Lock-Free Stack
---------------

I used C++ as my language of choice using std=c++11. I coded the project on a folder in Windows 10, using Atom as my IDE.

When running my program, the following command in a linux/ubuntu ternimal.

g++ -pthread Stack.cpp

Followed by "./a.out", to run the given code

When compiling the program. you may get warnings about the creation of the Stack. These do not stop the build and ./a.out can run after (You can run --no-warnings to remove them). 

The result shows the time, number of operations, size of the stack and the amount of each call.

(-pthread) is a required flag to ensure that the code is run using concurrency instead of on one main thread

Dependencies
------------

thread - needed to create threads in the environment

iostream - mainly used for error handling and results

math.h - used to find random values

chrono - used to determine speed at which the threads completed in

atomic - atomic variables to ensure lock-free


Implementation
--------------

For my approach at the problem, I translated the psuedocode into a C++ file with two classes representing Node and Stack.

Node allows for the creation of generics through it's template. Stack follows a traditional implementation with the combination of atomics. All fields are listed as atomic, the head Node being one of them. The reason for this is to allow for the thread-safe operation of modifying an object.

**Push**

The implementation creates a new node (n) and get the pointer to the head node. Once the thread has gotten the head, it becomes the Node's next and the Stack head. Since the head is atomic, I can use the atomic operation of atomic::compare_exchange_weak to ensure that another thread hasn't modified the head by comparing it to the expected value.

Once the compare value is what is expected, the stack's fields are incremented and the method returns true.

**Pop**

The implementation grabs the object's head pointer and creates a temporary node to grab the next head. I use atomic::compare_exchange_weak to compare the head value to what is expected from the temp node. Once the head has been modified, the fields can be incremented and the previous head value can be returned.  

Resource - http://www.cplusplus.com/reference/atomic/atomic/compare_exchange_weak/

**Threads**

Threads are utilized with a method inside of the Stack class which randomly selects an operation to perform from push(), pop(), getSize().

Atomic Variables (Answer to Part 1.2)
----------------

For my approach to the stack, I made all the fields atomic. This includes the head pointer node and 5 integer counters. 

Node - The pointer to the head is crucial to be atomic as it maintains the data integrity of the stack. If not made atomic, it's possible for two threads to be modifying head and create a invalid address. Making the pointer atomic allows for faster referencing and thread-safe.

Counters - Two of the atomic integers are utilized for Part 1 and Part 2 of the assignment. Number of Operations is tracked with everytime there is a push, pop or size call. The size must be atomic as it has multiple threads modifying the value and it allows it to be thread-safe. The other three counters are for optional output for determining the specfic amount of calls. These fields need to be atomic to allow for thread-safe operations on the values.

These primitives and pointer would not want to be volitale as they are modified frequently and cannot garuntee atomicity during operations. Declaring the fields as atomic, it allows for all threads to modify any field safely. 

Graphs
------

I have created PDFs of graphs located in the directory. 

These names correspond to the resulting ratios (Push/Pop/Size):

33Graph.pdf - (33/33/33)

40Graph.pdf - (40/40/20)

50Graph.pdf - (50/50/0)

It's also important to note that I utilize the push functionality to pre-load nodes into the stack. 

There is a skew of 50,000 on push() because of this. Another thing to note is the getSize call at the end of my program for console logging. This is shown by the 1 of size() in 50Graph.pdf. 

