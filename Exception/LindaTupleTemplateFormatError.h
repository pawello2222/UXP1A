//
// Created by Kacper Harasim on 05.06.2016.
//

#ifndef UXP1A_LINDATUPLETEMPLATEFORMAT_H
#define UXP1A_LINDATUPLETEMPLATEFORMAT_H


#include "ErrorCodeMessageExceptionBase.h"

class LindaTupleTemplateFormatError : public ErrorCodeMessageExceptionBase
{
 public:
  LindaTupleTemplateFormatError(std::string message, int errorCode);
};


#endif //UXP1A_LINDATUPLETEMPLATEFORMAT_H
