//
// Created by Kacper Harasim on 08.06.2016.
//

#ifndef UXP1A_UNRECOGNIZEDTOKEN_H
#define UXP1A_UNRECOGNIZEDTOKEN_H

#include "MessageExceptionBase.h"
class UnrecognizedToken : public MessageExceptionBase
{
 public:
  UnrecognizedToken(std::string message): MessageExceptionBase(message) {}
};
#endif //UXP1A_UNRECOGNIZEDTOKEN_H
