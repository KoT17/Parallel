#include <thread>
#include <iostream>
#include <math.h>
#include <atomic>
#include <chrono>
#include <vector>

using namespace std;
using namespace chrono;

template <typename T>
class Node {
public:
  T val;
  Node* next;
  Node(T _val);
  Node();
};

template <typename T>
Node<T>::Node(T _val) {
  val = _val;
}

template <typename T>
Node<T>::Node() {
  val = NULL;
}


template <typename T>
class Stack {
public:
  atomic<Node<T>* > head;
  atomic<int> numOps;
  atomic<int> size;

  // Used for graphs
  atomic<int> sizeCalls;
  atomic<int> popCalls;
  atomic<int> pushCalls;

  Stack() {
    head = NULL;
    size = 0;
    numOps = 0;

    sizeCalls = 0;
    popCalls = 0;
    pushCalls = 0;
  }

  // Pushes generic T to head with CAS for safety
  bool push(T x) {
    Node<T>* n = new Node<T>(x);
    Node<T>* cur = head.load(memory_order_relaxed);

    while(1) {
      n->next = cur;
      if (head.compare_exchange_weak(cur, n)) {
        numOps++;
        size++;
        pushCalls++;
        return true;
      }
    }
    return false;

  }

  // Pops the head of the stack with CAS for safety
  T pop() {
    Node<T>* n = head.load(memory_order_relaxed);
    Node<T>* cur;
    do {
      if (n == NULL)
        return NULL;
      cur = n->next;
    } while (!head.compare_exchange_weak(n, cur, std::memory_order_relaxed));

    numOps++;
    size--;
    popCalls++;
    return n->val;
  }

  int getNumOps() {
      return numOps;
  }

  int getSize() {
      numOps++;
      sizeCalls++;
      return size;
  }

  // Thread method for randomly selecting operation
  void process_stack(int x) {
    for (int i = 0; i < 150000; i++) {
      int select = rand() % 3;
      if (select == 0)
        pop();
      else if (select == 1)
        push(rand());
      else
        getSize();
    }
  }
};


int main() {
  Stack<int> sta;
  vector<thread> thrs(4);

  // Prepopulating stack
  for (int i = 0; i < 50000; i++)
    sta.push(rand());

  auto start = high_resolution_clock::now();

  for (int i = 0; i < 4; i++)
		thrs[i] = thread(&Stack<int>::process_stack, &sta, i);
  for (int i = 0; i < 4; i++)
  	thrs[i].join();

  auto end = high_resolution_clock::now();

  cout << "Time: " << duration_cast<milliseconds>(end - start).count() << "ms" << endl;
  cout << "Num Of Operations: " << sta.getNumOps() << endl;
  cout << "Size Of Stack: " << sta.getSize() << endl;
  cout << "Amount Of Calls to Size(): " << sta.sizeCalls << endl;
  cout << "Amount Of Calls to Pop(): " << sta.popCalls << endl;
  cout << "Amount Of Calls to Push(): " << sta.pushCalls << endl;
}
