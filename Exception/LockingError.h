//
// Created by anowikowski on 28.05.16.
//

#ifndef UXP1A_LOCKINGERROR_H
#define UXP1A_LOCKINGERROR_H


#include "ErrorCodeMessageExceptionBase.h"

class LockingError : public ErrorCodeMessageExceptionBase
{
public:
    LockingError(std::string message, int errorCode);
};


#endif //UXP1A_LOCKINGERROR_H
