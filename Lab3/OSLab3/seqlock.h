#include <atomic>
#include <iostream>
class pthread_seqlock_t {
   std::atomic_int32_t *counter;
public:
   int getCounter() const {

   }
};
