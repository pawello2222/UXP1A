//
// Created by anowikowski on 27.05.16.
//

#include "LindaTuple.h"

LindaTuple::LindaTuple(std::vector<LindaTupleItem> lindaTupleItems) : m_lindaTupleItems(lindaTupleItems)
{

}

std::vector<LindaTupleItem>& LindaTuple::GetItems()
{
    return this->m_lindaTupleItems;
}

std::string LindaTuple::ToString()
{
    std::string delimiter = ",";
    std::string str = "(";
    for (auto itemIt = this->m_lindaTupleItems.begin(); itemIt != this->m_lindaTupleItems.end(); ++itemIt)
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
