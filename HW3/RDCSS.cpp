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


uintptr_t CAS1(atomic<uintptr_t> *address, uintptr_t old, uintptr_t n) { // GOOD
  atomic_compare_exchange_strong(address, &old, n); // Can be  address.compare_exchange_strong()
  return old;
}

bool isDescriptor(uintptr_t address) { // use reinterpret_cast to optional object GOOD
  //cout << "D: (isDescriptor) Address is: " << address << endl; // May need use reinterpret_cast in to uintptr_t in isDescriptor
  if (address & 1)
    return true;
  return false;
}

void Complete (Descriptor *des) {
  uintptr_t temp = atomic_load(des->a1);// load control address Does it need to be a shared pointer
  if (temp == des->o1) {
    CAS1(des->a2, (reinterpret_cast<uintptr_t>(des) | 0x01), des->newV); // Change to 0x01 just in case
  } else {
    CAS1(des->a2, (reinterpret_cast<uintptr_t>(des) | 0x01), des->o2);
  }
}

uintptr_t RDCSSRead(atomic<uintptr_t> *address) {
  uintptr_t result;
  //cout << "Hello" << endl;
  do {
    result = atomic_load(address);// atomic load for value
    //cout << result << endl;
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
      Complete(reinterpret_cast<Descriptor*>(result & ~0x01)); // May need to make result to uintptr_t
  } while (isDescriptor(result));

  if (result == des->o2)
    Complete(des);
  return result;
}

void controlTimer() {
    auto start = high_resolution_clock::now();
    while (duration_cast<seconds>(high_resolution_clock::now() - start).count() < 10) {}

    flag.store((uintptr_t)new bool(true)); // May need to cast to uintptr_t
}

void populateRDCSS() {
  //cout << "Flag: " << *(bool*)flag.load() << endl;
  while(!*(bool*)atomic_load(&flag)) { // bool* may be neccessary but also not
    Descriptor* des = new Descriptor();

    des->a1 = &flag;
    des->a2 = &counter;

    des->o1 = flag.load();
    des->o2 = RDCSSRead(des->a2);


    unsigned long *temp = new unsigned long(*(unsigned long*)des->o2 + 1);

    des->newV = (uintptr_t)temp;

    /*cout << "des.a1 (" << des->a1 << ") des.a2 (" << des->a2 << ")" << endl;
    cout << "des.o1 (" << des->o1 << ") des.o2 (" << des->o2 << ")" << endl;
    cout << "des.newV (" << des->newV << ")" << endl;*/

    RDCSS(des);
  }
}

int main() {

  vector<thread> thrs;
  counter.store((uintptr_t)new unsigned long(0));
  flag.store((uintptr_t)new bool(false)); // for o2 use RDCSSRead(counter address)

  uintptr_t ptr = 95645431;
  //cout << isDescriptor(ptr) << " " << counter << " " << flag <<  endl;

  thread test = thread(populateRDCSS);


  thread control = thread(controlTimer);

  test.join();
  control.join();

  cout << "Counter: " << *(unsigned long*)counter.load() << endl;
  return 0;
}
