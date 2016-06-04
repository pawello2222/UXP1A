//
// Created by Kacper Harasim on 30.05.2016.
//

#ifndef UXP1A_SEMAPHOREHANDLER_H
#define UXP1A_SEMAPHOREHANDLER_H


#include <string>
#include <semaphore.h>
#include <chrono>
class SemaphoreManager {

 private:
  static std::string semaphoreIdentifierForProcessWithId(int id);
  static sem_t *getSemaphore(std::string identifier);

 public:

  //Returns 0 if it was unlocked by different process, -1 if through timeout mechanism.
  static int LockOnSemaphoreWithTimeout(unsigned long timeout);
  static void LockOnSemaphore();
  static void UnlockSemaphoreWithProcessId(int id);
};


#endif //UXP1A_SEMAPHOREHANDLER_H
