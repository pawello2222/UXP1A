//
// Created by Pawel Wiszenko on 09.05.2016.
//

#ifndef UXP1A_ELEMENT_H
#define UXP1A_ELEMENT_H


#include <string>

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

#endif //UXP1A_ELEMENT_H
