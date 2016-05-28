//
// Created by anowikowski on 28.05.16.
//

#include "FileOperationError.h"

FileOperationError::FileOperationError(std::string message, int errorCode) : ErrorCodeMessageExceptionBase(message, errorCode)
{

}
