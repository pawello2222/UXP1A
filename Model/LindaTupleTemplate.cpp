//
// Created by anowikowski on 28.05.16.
//

#include "LindaTupleTemplate.h"

LindaTupleTemplate::LindaTupleTemplate(std::vector<LindaTupleItemTemplate> itemTemplates) : m_itemTemplates(itemTemplates)
{

}

bool LindaTupleTemplate::IsMatch(LindaTuple &lindaTuple)
{
    if (this->m_itemTemplates.size() != lindaTuple.GetItems().size())
    {
        return false;
    }

    std::vector<LindaTupleItemTemplate>::iterator itemTemplate;
    std::vector<LindaTupleItem>::iterator tupleItem;
    for (itemTemplate = this->m_itemTemplates.begin(), tupleItem = lindaTuple.GetItems().begin(); itemTemplate != this->m_itemTemplates.end(); ++itemTemplate, ++tupleItem)
    {
        if (!itemTemplate->IsMatch(*tupleItem))
        {
            return false;
        }
    }

    return true;
}


