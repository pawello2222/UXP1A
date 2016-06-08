//
// Created by Kacper Harasim on 05.06.2016.
//

#ifndef UXP1A_LINDATUPLETEMPLATEFORMAT_H
#define UXP1A_LINDATUPLETEMPLATEFORMAT_H


#include "MessageExceptionBase.h"

class LindaTupleTemplateFormatError : public MessageExceptionBase
{
 public:
  LindaTupleTemplateFormatError(std::string message);
};


#endif //UXP1A_LINDATUPLETEMPLATEFORMAT_H
