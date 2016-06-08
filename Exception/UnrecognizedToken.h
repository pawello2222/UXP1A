//
// Created by Kacper Harasim on 08.06.2016.
//

#ifndef UXP1A_UNRECOGNIZEDTOKEN_H
#define UXP1A_UNRECOGNIZEDTOKEN_H

#include "ErrorCodeMessageExceptionBase.h"
class UnrecognizedToken : public ErrorCodeMessageExceptionBase
{
 public:
  UnrecognizedToken(std::string message, int errorCode): ErrorCodeMessageExceptionBase(message, errorCode) {}
};
#endif //UXP1A_UNRECOGNIZEDTOKEN_H
