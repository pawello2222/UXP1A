//
// Created by Kacper Harasim on 30.05.2016.
//

#include <zconf.h>
#include <semaphore.h>
#include <cstdlib>
#include <fcntl.h>
#include <ctime>
#include <unistd.h>
#include "../Utilities/TimeOSX.h"
#include "SemaphoreManager.h"
#include "../Exception/SemaphoreLockingError.h"
#include "../Utilities/TimeUtilities.h"
#include "../Utilities/OSXSemTimeout.h"

std::string SemaphoreManager::semaphoreIdentifierForProcessWithId(int id) {
  return "UXP_SEM" + std::to_string(id);
}

sem_t* SemaphoreManager::getSemaphore(std::string identifier) {
  sem_t* mutex = sem_open(identifier.c_str(), O_CREAT, 0644, 0);
  if (mutex == SEM_FAILED) {
    perror("semaphore initialization");
    exit(1);
  }
  return mutex;
}

int SemaphoreManager::LockOnSemaphoreWithTimeout(unsigned long timeout) {
  struct timespec currentTimespec;
  if( clock_gettime( CLOCK_REALTIME, &currentTimespec) == -1 ) {
    throw SemaphoreLockingError("Cannot get current time while setting semaphore timeout", errno);
  }

  struct timespec millisecondsTimespec;
  TimeUtilities::millisecondsToTimespec(&millisecondsTimespec, timeout);
  struct timespec result;
  TimeUtilities::addTimespecs(currentTimespec, millisecondsTimespec, result);

  int pid = getpid();
  auto semId = semaphoreIdentifierForProcessWithId(pid);
  sem_t* mutex = getSemaphore(semId);
  return sem_timedwait(mutex, &result);
}

void SemaphoreManager::LockOnSemaphore() {
  int pid = getpid();
  auto semId = semaphoreIdentifierForProcessWithId(pid);
  sem_t* mutex = getSemaphore(semId);
  sem_wait(mutex);
}

void SemaphoreManager::UnlockSemaphoreWithProcessId(int id) {
  auto semId = semaphoreIdentifierForProcessWithId(id);
  sem_t* mutex = getSemaphore(semId);
  sem_post(mutex);
}
