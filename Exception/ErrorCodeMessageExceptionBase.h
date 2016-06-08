//
// Created by anowikowski on 28.05.16.
//

#ifndef UXP1A_ERRORCODEMESSAGEEXCEPTIONBASE_H
#define UXP1A_ERRORCODEMESSAGEEXCEPTIONBASE_H


#include <string>
#include "MessageExceptionBase.h"

class ErrorCodeMessageExceptionBase : public MessageExceptionBase
{
public:
    ErrorCodeMessageExceptionBase(std::string message, int errorCode);
    int GetErrorCode();
protected:
    int m_iErrorCode;
};


#endif //UXP1A_ERRORCODEMESSAGEEXCEPTIONBASE_H
