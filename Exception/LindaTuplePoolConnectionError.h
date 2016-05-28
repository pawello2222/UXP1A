//
// Created by anowikowski on 28.05.16.
//

#ifndef UXP1A_LINDATUPLEPOOLCONNECTIONERROR_H
#define UXP1A_LINDATUPLEPOOLCONNECTIONERROR_H


#include <exception>
#include <string>

class LindaTuplePoolConnectionError : public std::exception
{
public:
    LindaTuplePoolConnectionError(std::string message, int errorCode);
    std::string GetMessage();
    int GetErrorCode();
private:
    std::string m_sMessage;
    int m_iErrorCode;
};


#endif //UXP1A_LINDATUPLEPOOLCONNECTIONERROR_H
