#include <thread>
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>

using namespace std;

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
  Node<T> *head;
  int numOps;

  Stack() {
    head = NULL;
  }

  bool push(T x) {
    Node<T>* n = new Node<T>(x);
    n->next = head;
    head = n;
    numOps++;
    return true;
  }

  T pop() {
    if (head == NULL) {
      return NULL;
    }
    Node<T> *n = head;
    head = n->next;
    numOps++;
    return n->val;
  }

  int getNumOps() {
      return numOps;
  }
};


int main() {
  Stack<int> sta;
  vector<thread> thrs(8);


}
