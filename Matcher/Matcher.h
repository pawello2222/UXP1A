//
// Created by Pawel Wiszenko on 28.04.2016.
//

#ifndef UXP1A_MATCHER_H
#define UXP1A_MATCHER_H


#include <string>
#include <vector>
#include <iostream>

//todo move data structures to another directory
class Element
{
public:
    Element( int _i ) : type( "integer" ), i( _i ) {};
    Element( float _f ) : type( "float" ), f( _f ) {};
    Element( std::string _s ) : type( "string" ), s( _s ) {};

    std::string type;
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
    Requirement( std::string _type, std::string _value ) : type( _type ), value( _value ) {};

    std::string type;
    std::string value;
};


class Matcher
{
public:
    static Matcher& getInstance();
    Matcher( Matcher const& ) = delete;
    void operator=( Matcher const& ) = delete;

    bool match( Element* element );
    bool areRequirementsMet();

    void setRequirements( std::vector< Requirement* > _requirements);

private:
    Matcher();

    std::vector< Requirement* > requirements;
};


#endif //UXP1A_MATCHER_H
