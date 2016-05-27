//
// Created by Pawel Wiszenko on 28.04.2016.
//

#ifndef UXP1A_MATCHER_H
#define UXP1A_MATCHER_H


#include <string>
#include <vector>
#include <iostream>
#include "../Model/LindaTupleItemType.h"

//todo move data structures to another directory
class Element
{
public:
    Element( int _i ) : type(LindaTupleItemType::Integer), i( _i ) {};
    Element( float _f ) : type(LindaTupleItemType::Float), f( _f ) {};
    Element( std::string _s ) : type(LindaTupleItemType::String), s( _s ) {};

    LindaTupleItemType type;
    union
    {
        int i;
        float f;
        std::string s;
    };
};

class Requirement
{
public:
    Requirement( LindaTupleItemType _type, std::string _value ) : type( _type ), value( _value ) {};

    LindaTupleItemType type;
    std::string value;
};


class Matcher
{
public:
    Matcher( std::vector< Requirement* > _requirements );
    ~Matcher();

    bool match( Element* element );
    bool areRequirementsMet();

private:
    std::vector< Requirement* > requirements;
};


#endif //UXP1A_MATCHER_H
