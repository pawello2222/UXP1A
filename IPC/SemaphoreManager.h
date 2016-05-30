//
// Created by Kacper Harasim on 30.05.2016.
//

#ifndef UXP1A_SEMAPHOREHANDLER_H
#define UXP1A_SEMAPHOREHANDLER_H


#include <string>
#include <semaphore.h>
class SemaphoreManager {

 private:
  static std::string semaphoreIdentifierForProcessWithId(int id);
  static sem_t *getSemaphore(std::string identifier);

 public:
  static void LockOnSemaphore();
  static void UnlockSemaphoreWithProcessId(int id);
};


#endif //UXP1A_SEMAPHOREHANDLER_H
