//
// Created by Kacper Harasim on 04.06.2016.
//

#ifndef UXP1A_SEMAPHORELOCKINGERROR_H
#define UXP1A_SEMAPHORELOCKINGERROR_H


#include "ErrorCodeMessageExceptionBase.h"
class SemaphoreLockingError : public ErrorCodeMessageExceptionBase
{
 public:
  SemaphoreLockingError(std::string message, int errorCode): ErrorCodeMessageExceptionBase(message, errorCode) {}
};
#endif //UXP1A_SEMAPHORELOCKINGERROR_H

