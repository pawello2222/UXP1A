//
// Created by Pawel Wiszenko on 28.04.2016.
//

#include <iostream>
#include "Matcher/Matcher.h"

using namespace std;

int main()
{
    Matcher& matcher = Matcher::getInstance();

    std::vector< Requirement* > reqs = { new Requirement( "integer", "1" ), new Requirement( "string", "ASD" ) };
    matcher.setRequirements( reqs );

    matcher.match( new Element( "ASD" ) ); // if one returns false -> no match
    matcher.match( new Element( 1 ) );
//    matcher->match( new Element( (float)1.0 ) );

    std::cout << matcher.areRequirementsMet() << std::endl;
    // delete unused requirements

    reqs = { new Requirement( "string", "*" ), new Requirement( "float", "1.0" ) };
    matcher.setRequirements( reqs );

    matcher.match( new Element( "ASD" ) ); // if one returns false -> no match
    matcher.match( new Element( 1 ) );
//    matcher.match( new Element( (float)1.0 ) );

    std::cout << matcher.areRequirementsMet() << std::endl;
    // delete unused requirements

    return 0;
}