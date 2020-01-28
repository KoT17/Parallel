#include <string>
#include <mutex>
#include <iostream>

#include "philosopher.h"

using namespace std;

Philosopher::Philosopher() {
  ID = -1;
  left = -1;
  right = -1;
}

Philosopher::Philosopher(int num, int leftPos, int rightPos) {
  ID = num;
  left = leftPos;
  right = rightPos;
}
