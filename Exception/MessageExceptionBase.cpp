//
// Created by anowikowski on 08.06.16.
//

#include "MessageExceptionBase.h"

MessageExceptionBase::MessageExceptionBase() : m_sMessage("")
{

}

MessageExceptionBase::MessageExceptionBase(std::string message) : m_sMessage(message)
{

}

std::string MessageExceptionBase::GetMessage()
{
    return this->m_sMessage;
}
