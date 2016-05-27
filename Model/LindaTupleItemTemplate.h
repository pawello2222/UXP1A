//
// Created by anowikowski on 28.05.16.
//

#ifndef UXP1A_LINDATUPLEITEMTEMPLATE_H
#define UXP1A_LINDATUPLEITEMTEMPLATE_H


#include <string>
#include "LindaTupleItemType.h"
#include "LindaTuple.h"

class LindaTupleItemTemplate
{
public:
    LindaTupleItemTemplate(LindaTupleItemType type, std::string value);
    LindaTupleItemType GetType();
    std::string GetValue();
    bool IsMatch(LindaTupleItem &lindaTupleItem);
private:
    LindaTupleItemType m_type;
    std::string m_value;
};


#endif //UXP1A_LINDATUPLEITEMTEMPLATE_H
