//
// Created by Pawel Wiszenko on 28.04.2016.
//

#include "Matcher.h"

Matcher& Matcher::getInstance()
{
    static Matcher instance;

    return instance;
}

Matcher::Matcher()
{

}

bool Matcher::match( Element* element )
{
    bool matched = false;

    for ( auto it = requirements.begin(); it != requirements.end(); ++it )
    {
        if ( ( *it )->type == element->type )
        {
            if ( ( *it )->value == "*"
                 || ( *it )->value == element->s
                 || stoi( ( *it )->value ) == element->i
                 || stof( ( *it )->value ) == element->f )
            {
                matched = true;
                requirements.erase( it-- );
            }
        }
    }

    return matched;
}

bool Matcher::areRequirementsMet()
{
    return !requirements.size();
}

void Matcher::setRequirements( std::vector< Requirement* > _requirements )
{
    this->requirements = _requirements;
}