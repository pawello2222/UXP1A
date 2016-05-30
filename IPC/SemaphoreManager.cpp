//
// Created by Kacper Harasim on 30.05.2016.
//

#include <zconf.h>
#include <semaphore.h>
#include <cstdlib>
#include "SemaphoreManager.h"

std::string SemaphoreManager::semaphoreIdentifierForProcessWithId(int id) {
  return "UXP_SEM" + std::to_string(id);
}

sem_t* SemaphoreManager::getSemaphore(std::string identifier) {
  sem_t* mutex = sem_open(identifier.c_str(), O_CREAT, 0644, 1);
  if (mutex == SEM_FAILED) {
    perror("semaphore initilization");
    exit(1);
  }
  return mutex;
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