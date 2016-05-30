//
// Created by Kacper Harasim on 30.05.2016.
//

#ifndef UXP1A_SEMAPHOREHANDLER_H
#define UXP1A_SEMAPHOREHANDLER_H


#include <string>
class SemaphoreManager {

 private:
  static std::string semaphoreIdentifierForProcessWithId(int id);
  sem_t *getSemaphore(std::string identifier);

 public:
  void LockOnSemaphore();
  void UnlockSemaphoreWithProcessId(int id);
};


#endif //UXP1A_SEMAPHOREHANDLER_H
