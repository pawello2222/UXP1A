//
// Created by anowikowski on 27.05.16.
//

#ifndef UXP1A_INVALIDOPERATION_H
#define UXP1A_INVALIDOPERATION_H


#include <exception>
#include "MessageExceptionBase.h"

class InvalidOperation : public MessageExceptionBase
{
public:
    InvalidOperation();
    InvalidOperation(std::string message);

};


#endif //UXP1A_INVALIDOPERATION_H
