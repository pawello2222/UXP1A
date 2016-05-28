//
// Created by anowikowski on 28.05.16.
//

#include "LindaTuplePoolConnectionError.h"

LindaTuplePoolConnectionError::LindaTuplePoolConnectionError(std::string message, int errorCode) : m_sMessage(message), m_iErrorCode(errorCode)
{

}

std::string LindaTuplePoolConnectionError::GetMessage()
{
    return this->m_sMessage;
}

int LindaTuplePoolConnectionError::GetErrorCode()
{
    return this->m_iErrorCode;
}




