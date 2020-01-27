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
void find_primes(bitset<limit>* sieve, vector<int> numbers, int index);

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

  // Is this a bottle neck due to only spawning one thread at a time?
  for (int i = 0; i < numbers.size(); i+=8) {

    cout << "Number: " << numbers.at(i) << endl;

    for(int j = 0; j < 8; j++)
      thrs[j] = thread(find_primes, &sieve, numbers, i+j);

    for(int j = 0; j < 8; j++)
      thrs[j].join();
  }

  auto end = high_resolution_clock::now();
  cout << "Threads executed in " << duration_cast<milliseconds>(end - start).count() << " milliseconds" << endl;
  return 0;
}

void find_primes(bitset<limit>* sieve, vector<int> numbers, int index) {

  int num = numbers.at(index);
  if(sieve->test(index) == 1) {
    cout << "Skipped " << num << endl;
    return;
  }

  cout << "Checking number: " << num << endl;
  if(is_prime(num)) {
    sieve->set(index, 0);
    cout << "ISPRIME" << endl;

    for (int i = index+1; i < sieve->size(); i++) {
      if((numbers.at(i) % num) == 0)
        sieve->set(i,1);
    }
  }
  // Update sieve here
  cout << "Passed sieve on: " << num << endl;
}

bool is_prime(int number) {
  for (int i = 4; i < sqrt(number) ; i++){
    if(number % i == 0)
      return false;
  }
  return true;
}
