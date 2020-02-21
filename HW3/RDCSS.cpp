#include <thread>
#include <iostream>
#include <cstdint>
#include <atomic>
#include <chrono>
#include <vector>

using namespace std;
using namespace chrono;

atomic<int> counter;
atomic<bool> flag;

class Descriptor {
public:
  int *a1;
  int *a2;
  int o1;
  int o2;
  int newV;
};

uintptr_t RDCSS(Descriptor *r) {

}

void controlTimer() {
    auto start = high_resolution_clock::now();
    while (duration_cast<seconds>(high_resolution_clock::now() - start).count() < 75) {}
    flag = true;
}

int main() {

  vector<thread> thrs;
  counter = 0;
  flag = false;

  thread control = thread(controlTimer);
  control.join();

  cout << "sup" << endl;
  return 0;
}
