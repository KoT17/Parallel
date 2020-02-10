#include <string>
#include <iostream>

using namespace std;

template <typename T>
class Node {
public:
  T val;
  Node* next;
  Node(T _val);
  Node();
};
