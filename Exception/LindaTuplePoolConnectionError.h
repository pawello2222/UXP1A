//
// Created by anowikowski on 28.05.16.
//

#ifndef UXP1A_LINDATUPLEPOOLCONNECTIONERROR_H
#define UXP1A_LINDATUPLEPOOLCONNECTIONERROR_H


#include <exception>
#include <string>
#include "ErrorCodeMessageExceptionBase.h"

class LindaTuplePoolConnectionError : public ErrorCodeMessageExceptionBase
{
public:
    LindaTuplePoolConnectionError(std::string message, int errorCode);
};


#endif //UXP1A_LINDATUPLEPOOLCONNECTIONERROR_H
