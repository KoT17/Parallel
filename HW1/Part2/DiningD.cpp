#include <iostream>
#include <mutex>
#include <vector>
#include <thread>
#include <chrono>
#include <atomic>
#include <cstring>

#include "chopstick.h"
#include "philosopher.h"

using namespace std;

void eat(Philosopher* thinker, Chopstick* left, Chopstick* right);
void exitKeyPressed();

atomic<bool> keyPressed;

int main(int argc, const char* argv[]) {
  int passedNum = atoi(argv[1]);
  vector<Chopstick> sticks(passedNum);
  vector<Philosopher> philo;
  vector<thread> thrs(passedNum);

  for (int i = 0; i < passedNum; i++) {
    Philosopher temp(i,i,i+1);
    if (i == passedNum-1)
      temp.right = 0;
    philo.push_back(temp);
  }

  // Had a temp object beforehand and that resulted in multiple threads modifying variable
  thread press = thread(exitKeyPressed);

  while(!keyPressed) {
    for (int i = 0; i < passedNum; i++) {
      thrs[i] = thread(eat, &philo.at(i), &sticks[philo.at(i).left], &sticks[philo.at(i).right]);
    }
    for (int i = 0; i < passedNum; i++)
      thrs[i].join();
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
