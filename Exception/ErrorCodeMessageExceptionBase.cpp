//
// Created by anowikowski on 28.05.16.
//

#include "ErrorCodeMessageExceptionBase.h"

ErrorCodeMessageExceptionBase::ErrorCodeMessageExceptionBase(std::string message, int errorCode) : m_sMessage(message), m_iErrorCode(errorCode)
{

}

std::string ErrorCodeMessageExceptionBase::GetMessage()
{
    return this->m_sMessage;
}

int ErrorCodeMessageExceptionBase::GetErrorCode()
{
    return this->m_iErrorCode;
}
