//
// Created by Kacper Harasim on 08.06.2016.
//

#ifndef UXP1A_UNKNOWNLINDATEMPLATEVALUE_H
#define UXP1A_UNKNOWNLINDATEMPLATEVALUE_H


#include "MessageExceptionBase.h"
class UnknownLindaTemplateValue : public MessageExceptionBase
{
public:

  UnknownLindaTemplateValue(const std::string &message) : MessageExceptionBase(message) { }
};


#endif //UXP1A_UNKNOWNLINDATEMPLATEVALUE_H
