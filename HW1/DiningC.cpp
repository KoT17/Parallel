#include <iostream>
#include <mutex>
#include <vector>
#include <thread>
#include <chrono>
#include <cstring>
#include <atomic>
#include <queue>

#include "chopstick.h"
#include "philosopher.h"

using namespace std;

void eat(Philosopher* thinker, Chopstick* left, Chopstick* right);
void exitKeyPressed();

atomic<bool> keyPressed;

int main() {
  vector<Chopstick> sticks(5);
  vector<Philosopher> philo;
  vector<thread> thrs(5);
  queue<int> pQueue;

  keyPressed = 0;

  for (int i = 0; i < 5; i++) {
    Philosopher temp(i,i,i+1);
    if (i == 4)
      temp.right = 0;
    philo.push_back(temp);
  }

  thread press = thread(exitKeyPressed);

  for (int i = 0; i < 5; i++) {
    thrs[i] = thread(eat, &philo.at(i), &sticks[philo.at(i).left], &sticks[philo.at(i).right]);
  }

  for (int i = 0; i < 5; i++) {
    thrs[i].join();
    pQueue.push(i);
  }
  while(!keyPressed) {
    int popped;
    while(!pQueue.empty()){
      popped = pQueue.front();
      pQueue.pop();
      thrs[popped] = thread(eat, &philo.at(popped), &sticks[philo.at(popped).left], &sticks[philo.at(popped).right]);
      popped = pQueue.front();
    }
    for (int i = 0; i < 5; i++) {
      thrs[i].join();
      pQueue.push(i);
    }
  }
  press.join();

  return 0;
}

void exitKeyPressed() {
  char* temp;
  cin >> temp;
  while(strcmp(temp, "n")){
    cin >> temp;
  }
  keyPressed = 1;
}

// Need to pass the pointer due to making sure the vector object is locked
// locking both chopsticks on the same line allows for avoiding deadlock
void eat(Philosopher* thinker, Chopstick* left, Chopstick* right) {
  cout << thinker->ID << " is now hungry" << endl;
  lock(left->mtx, right->mtx);
  cout << thinker->ID << " is now eating" << endl;
  lock_guard<mutex> leftGuard(left->mtx, adopt_lock);
  lock_guard<mutex> rightGuard(right->mtx, adopt_lock);

  this_thread::sleep_for(chrono::milliseconds(500));
  cout << thinker->ID << " is now thinking." << endl;
}
