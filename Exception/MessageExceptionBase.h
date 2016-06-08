//
// Created by anowikowski on 08.06.16.
//

#ifndef UXP1A_MESSAGEEXCEPTIONBASE_H
#define UXP1A_MESSAGEEXCEPTIONBASE_H


#include <string>

class MessageExceptionBase : std::exception
{
public:
    MessageExceptionBase();
    MessageExceptionBase(std::string message);
    std::string GetMessage();
protected:
    std::string m_sMessage;
};


#endif //UXP1A_MESSAGEEXCEPTIONBASE_H
