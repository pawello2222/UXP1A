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

std::string LindaTupleTemplate::ToString()
{
    std::string delimiter = ",";
    std::string str = "(";
    for (auto itemIt = this->m_itemTemplates.begin(); itemIt != this->m_itemTemplates.end(); ++itemIt)
    {
        str += itemIt->FormatValueAsFileEntryData() + delimiter;
    }

    //Check if ends with delimiter, if true then remove it from the end
    if (str.length() >= delimiter.length() && str.compare(str.length() - delimiter.length(), delimiter.length(), delimiter) == 0)
    {
        str.erase(str.end() - delimiter.length(), str.end());
    }

    str += ")";
    return str;
}
