//
// Created by anowikowski on 28.05.16.
//

#include "ErrorCodeMessageExceptionBase.h"

ErrorCodeMessageExceptionBase::ErrorCodeMessageExceptionBase(std::string message, int errorCode) : MessageExceptionBase(message), m_iErrorCode(errorCode)
{

}

int ErrorCodeMessageExceptionBase::GetErrorCode()
{
    return this->m_iErrorCode;
}
