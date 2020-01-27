#include <thread>
#include <iostream>
#include <math.h>
#include <vector>
#include <atomic>
#include <bits/stdc++.h>
#include <chrono>
#include <future>
#include <string>

using namespace std;
using namespace chrono;

#define limit 26666665

bool is_prime(int number);
void find_primes(bitset<limit> sieve, vector<int> numbers, int index, promise<bitset<limit> > *promise);

int main() {

  cout << "hello" << endl;
  int cap = pow(10,8), count = 2;

  vector<int> numbers;
  bitset<limit> sieve;

  while (count < cap) {
    if(!(count % 2 == 0) && !(count % 3 == 0) && !(count % 5 == 0))
      numbers.push_back(count);
    count++;
  }
  count = 0;

  cout << numbers.size() << endl;
  auto start = high_resolution_clock::now();

  vector<thread> thrs(8);
  promise<bitset<limit> > updatedSieve;
  future<bitset<limit> > future = updatedSieve.get_future();
  // Is this a bottle neck due to only spawning one thread at a time?
  for (int i = 0; i < numbers.size(); i+=8) {

    cout << "Number: " << numbers.at(i) << endl;
    for (int j = 0; j < 8; j++) {
      if (i+j >= numbers.size())
        break;
      thrs[j] = thread(find_primes, sieve, numbers, i+j, &updatedSieve);
    }
    //sieve = future.get();
    for (int j = 0; j < 8; j++) {
      thrs[j].join();
    }
    //sieve = future.get();
  }

  auto end = high_resolution_clock::now();
  cout << "Threads executed in " << duration_cast<milliseconds>(end - start).count() << " milliseconds" << endl;
  return 0;
}

void find_primes(bitset<limit> sieve, vector<int> numbers, int index, promise<bitset<limit> > *promise) {

  int num = numbers.at(index);
  if(sieve[index] == 1) {
    promise->set_value(sieve);
    return;
  }

  cout << "Checking number: " << num << endl;
  if(is_prime(num)) {
    sieve[index] = 0;
    cout << "ISPRIME" << endl;

    for (int i = index+1; i < sieve.size(); i++) {
      if(numbers.at(i) % num == 0)
        sieve[i] = 1;
    }
  }
  // Update sieve here
  cout << "Passed sieve on: " << num << endl;
  promise->set_value(sieve);
}

bool is_prime(int number) {
  for (int i = 4; i < sqrt(number) ; i++){
    if(number % i == 0)
      return false;
  }
  return true;
}
