//
// Created by anowikowski on 28.05.16.
//

#ifndef UXP1A_ERRORCODEMESSAGEEXCEPTIONBASE_H
#define UXP1A_ERRORCODEMESSAGEEXCEPTIONBASE_H


#include <string>

class ErrorCodeMessageExceptionBase : public std::exception
{
public:
    ErrorCodeMessageExceptionBase(std::string message, int errorCode);
    std::string GetMessage();
    int GetErrorCode();
protected:
    std::string m_sMessage;
    int m_iErrorCode;
};


#endif //UXP1A_ERRORCODEMESSAGEEXCEPTIONBASE_H
