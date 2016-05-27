//
// Created by anowikowski on 28.05.16.
//

#ifndef UXP1A_LINDATUPLETEMPLATE_H
#define UXP1A_LINDATUPLETEMPLATE_H


#include <vector>
#include "LindaTupleItemTemplate.h"

class LindaTupleTemplate
{
public:
    LindaTupleTemplate(std::vector<LindaTupleItemTemplate> itemTemplates);
    bool IsMatch(LindaTuple &lindaTuple);
private:
    std::vector<LindaTupleItemTemplate> m_itemTemplates;
};


#endif //UXP1A_LINDATUPLETEMPLATE_H
