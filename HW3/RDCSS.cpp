#include <thread>
#include <iostream>
#include <cstdint>
#include <atomic>
#include <chrono>
#include <vector>

using namespace std;
using namespace chrono;

atomic<uintptr_t> counter;
atomic<uintptr_t> flag;

class Descriptor {
public:
  atomic<uintptr_t> *a1;
  atomic<uintptr_t> *a2;
  uintptr_t o1;
  uintptr_t o2;
  uintptr_t newV;
};


uintptr_t CAS1(atomic<uintptr_t> *address, uintptr_t old, uintptr_t n) {
  atomic_compare_exchange_strong(address, &old, n);
  return old;
}

bool isDescriptor(uintptr_t address) {
  if (address & 1)
    return true;
  return false;
}

void Complete (Descriptor *des) {
  uintptr_t temp = atomic_load(des->a1);

  if (temp == des->o1) {
    CAS1(des->a2, (reinterpret_cast<uintptr_t>(des) | 0x01), des->newV);
  } else {
    CAS1(des->a2, (reinterpret_cast<uintptr_t>(des) | 0x01), des->o2);
  }
}

uintptr_t RDCSSRead(atomic<uintptr_t> *address) {
  uintptr_t result;

  do {
    result = atomic_load(address);

    if (isDescriptor(result))
      Complete(reinterpret_cast<Descriptor*>(result & ~0x01));
  } while (isDescriptor(result));

  return result;
}



uintptr_t RDCSS(Descriptor *des) {
  uintptr_t result;

  do {
    result = CAS1(des->a2, des->o2, (reinterpret_cast<uintptr_t>(des) | 0x01));
    if (isDescriptor(result))
      Complete(reinterpret_cast<Descriptor*>(result & ~0x01));
  } while (isDescriptor(result));

  if (result == des->o2)
    Complete(des);
  return result;
}

void controlTimer() {
    auto start = high_resolution_clock::now();
    while (duration_cast<seconds>(high_resolution_clock::now() - start).count() < 75) {}

    flag.store((uintptr_t)new bool(true));
}

void populateRDCSS() {
  while(!*(bool*)atomic_load(&flag)) {

    Descriptor* des = new Descriptor();

    des->a1 = &flag;
    des->a2 = &counter;

    des->o1 = flag.load();
    des->o2 = RDCSSRead(des->a2);

    unsigned long *temp = new unsigned long(*(unsigned long*)des->o2 + 1);
    des->newV = (uintptr_t)temp;

    RDCSS(des);
  }
}

int main() {

  vector<thread> thrs(16);
  counter.store((uintptr_t)new unsigned long(0));
  flag.store((uintptr_t)new bool(false));

  for(int i = 0; i < 16; i++)
    thrs[i] = thread(populateRDCSS);
  thread control = thread(controlTimer);

  for(int i = 0; i < 16; i++)
    thrs[i].join();
  control.join();

  cout << "Counter: " << *(unsigned long*)counter.load() << endl;
  return 0;
}
