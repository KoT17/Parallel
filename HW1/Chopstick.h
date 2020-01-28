#include <string>
#include <mutex>

using namespace std;

class Chopstick {
public:
  mutex mtx;

  Chopstick();
};
