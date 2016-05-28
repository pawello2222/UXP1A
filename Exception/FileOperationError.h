//
// Created by anowikowski on 28.05.16.
//

#ifndef UXP1A_FILEOPERATIONERROR_H
#define UXP1A_FILEOPERATIONERROR_H


#include "ErrorCodeMessageExceptionBase.h"

class FileOperationError : public ErrorCodeMessageExceptionBase
{
public:
    FileOperationError(std::string message, int errorCode);
};


#endif //UXP1A_FILEOPERATIONERROR_H
