//
// Created by Pawel Wiszenko on 09.05.2016.
//

#ifndef UXP1A_REQUIREMENT_H
#define UXP1A_REQUIREMENT_H


#include <string>

class Requirement
{
public:
    Requirement( std::string _type, std::string _value ) : type( _type ), value( _value ) {};

    std::string type;
    std::string value;
};


#endif //UXP1A_REQUIREMENT_H
