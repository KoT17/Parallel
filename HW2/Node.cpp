#include <stdio.h>
#include <string>
#include <iostream>
#include <cstring>

#include "Node.h"

using namespace std;

template <typename T>
Node<T>::Node(T _val) {
  val = _val;
}

template <typename T>
Node<T>::Node() {
  val = NULL;
}
