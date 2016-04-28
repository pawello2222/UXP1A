//
// Created by Pawel Wiszenko on 28.04.2016.
//

#include <iostream>
#include "Matcher/Matcher.h"

using namespace std;

int main()
{
    std::vector< Requirement* > reqs = { new Requirement( "integer", "1" ), new Requirement( "string", "ASD" ) };
    Matcher* matcher = new Matcher( reqs );
    matcher->match( new Element( "ASD" ) );
    matcher->match( new Element( 1 ) );
//    matcher->match( new Element( (float)1.0 ) );


    std::cout << matcher->areRequirementsMet() << std::endl;

    return 0;
}