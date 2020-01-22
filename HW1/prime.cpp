#include <thread>
#include <iostream>
#include <math.h>
#include <vector>
#include <atomic>
#include <queue>
#include <chrono>
#include <future>
#include <string>

using namespace std;
using namespace chrono;

bool is_prime(int number);
vector<bool> find_primes(vector<bool> sieve, int index);
vector<bool> update_sieve(vector<bool> sieve, int num);

int main() {

  cout << "hello" << endl;
  int cap = pow(10,8), count = 2;
  queue<int> numbers;
  vector<bool> sieve(cap, 0);

  while (count < cap) {
    if(!(count % 2 == 0) && !(count % 3 == 0) && !(count % 5 == 0))
      numbers.push(count);
    count++;
  }
  count = 0;

  sieve = update_sieve(sieve, 2);
  sieve = update_sieve(sieve, 3);
  sieve = update_sieve(sieve, 5);


  auto start = high_resolution_clock::now();

  vector<thread> thrs(8);

  // Is this a bottle neck due to only spawning one thread at a time?
  while (!numbers.empty()) {
    int temp = numbers.front();
    numbers.pop();
    cout << "Number: " << temp << endl;
    promise<vector<bool> > updatedSieve;
    auto future = updatedSieve.get_future();
    cout << "Passed future" << endl;
    thrs[count % 8] = thread(find_primes, sieve, temp);
    thrs[count % 8].join();

    sieve = future.get();
    count++;
  }


  auto end = high_resolution_clock::now();
  cout << "Threads executed in " << duration_cast<milliseconds>(end - start).count() << " milliseconds" << endl;
  return 0;
}

vector<bool> find_primes(vector<bool> sieve, int num) {

  if(sieve.at(num) == 1) {
    return sieve;
  }

  cout << "Checking number: " << num << endl;
  if(is_prime(num)) {
    sieve.at(num) == 1;
    cout << "ISPRIME" << endl;
  }

  // Update sieve here
  sieve = update_sieve(sieve, num);
  return sieve;
}

vector<bool> update_sieve(vector<bool> sieve, int num) {
  int temp = num;
  while (temp < sieve.size()) {
    if (sieve.at(temp) % num == 0)
      sieve.at(temp) = 1;
    temp++;
  }
  cout << "Passed sieve on: " << num << endl;
  return sieve;
}

bool is_prime(int number) {
  for (int i = 4; i < sqrt(number) ; i++){
    if(number % i == 0)
      return false;
  }
  return true;
}
