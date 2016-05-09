//
// Created by Pawel Wiszenko on 28.04.2016.
//

#ifndef UXP1A_MATCHER_H
#define UXP1A_MATCHER_H


#include <string>
#include <vector>
#include <iostream>
#include "Element.h"
#include "Requirement.h"

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
