//
// Created by Kacper Harasim on 01.06.2016.
//

#ifndef UXP1A_UNRECOGNIZEDCHARACTER_H
#define UXP1A_UNRECOGNIZEDCHARACTER_H

#include <string>
#include "MessageExceptionBase.h"
class UnrecognizedCharacter : public MessageExceptionBase
{
 public:
  UnrecognizedCharacter(std::string message): MessageExceptionBase(message) {}
};



#endif //UXP1A_UNRECOGNIZEDCHARACTER_H
