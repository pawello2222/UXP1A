//
// Created by Pawel Wiszenko on 28.04.2016.
//

#include <iostream>
#include "Matcher/Matcher.h"
#include "Model/LindaTupleItem.h"
#include "Model/LindaTuple.h"

using namespace std;

int main()
{
    std::vector< Requirement* > reqs = { new Requirement( LindaTupleItemType::Integer, "1" ), new Requirement( LindaTupleItemType::String, "ASD" ) };
    Matcher* matcher = new Matcher( reqs );


    LindaTupleItem l = LindaTupleItem(123);
    LindaTuple lt = LindaTuple({l});

    matcher->match( new Element( "ASD" ) ); // if one returns false -> no match
    matcher->match( new Element( 1 ) );
//    matcher->match( new Element( (float)1.0 ) );


    std::cout << matcher->areRequirementsMet() << std::endl;

    return 0;
}
