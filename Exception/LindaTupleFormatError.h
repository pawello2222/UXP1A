//
// Created by Kacper Harasim on 05.06.2016.
//

#ifndef UXP1A_LINDATUPLEFORMATERROR_H
#define UXP1A_LINDATUPLEFORMATERROR_H


#include "MessageExceptionBase.h"

class LindaTupleFormatError: public MessageExceptionBase
{
 public:
  LindaTupleFormatError(std::string message);
};


#endif //UXP1A_LINDATUPLEFORMATERROR_H
