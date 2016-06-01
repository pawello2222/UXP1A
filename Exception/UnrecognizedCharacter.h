//
// Created by Kacper Harasim on 01.06.2016.
//

#ifndef UXP1A_UNRECOGNIZEDCHARACTER_H
#define UXP1A_UNRECOGNIZEDCHARACTER_H

#include <string>
#include "ErrorCodeMessageExceptionBase.h"
class UnrecognizedCharacter : public ErrorCodeMessageExceptionBase
{
 public:
  UnrecognizedCharacter(std::string message, int errorCode): ErrorCodeMessageExceptionBase(message, errorCode) {}
};



#endif //UXP1A_UNRECOGNIZEDCHARACTER_H
