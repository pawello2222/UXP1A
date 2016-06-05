//
// Created by Kacper Harasim on 05.06.2016.
//

#ifndef UXP1A_LINDATUPLEFORMATERROR_H
#define UXP1A_LINDATUPLEFORMATERROR_H


#include "ErrorCodeMessageExceptionBase.h"

class LindaTupleFormatError: public ErrorCodeMessageExceptionBase
{
 public:
  LindaTupleFormatError(std::string message, int errorCode);
};


#endif //UXP1A_LINDATUPLEFORMATERROR_H
