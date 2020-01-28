#include <string>
#include <mutex>

using namespace std;

class Philosopher {
public:
  int left;
  int right;
  int ID; // Thread ID or "Philosopher"

  Philosopher();
  Philosopher(int num, int leftPos, int rightPos);
};
