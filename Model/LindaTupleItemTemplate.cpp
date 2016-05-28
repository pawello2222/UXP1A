//
// Created by anowikowski on 28.05.16.
//

#include "LindaTupleItemTemplate.h"
#include "../Exception/UnknownLindaTupleType.h"

LindaTupleItemTemplate::LindaTupleItemTemplate(LindaTupleItemType type, std::string value) : m_type(type), m_sValue(value)
{

}

LindaTupleItemType LindaTupleItemTemplate::GetType()
{
    return this->m_type;
}

std::string LindaTupleItemTemplate::GetValue()
{
    return this->m_sValue;
}

bool LindaTupleItemTemplate::IsMatch(LindaTupleItem &lindaTupleItem)
{
    if (this->m_type != lindaTupleItem.GetType())
    {
        return false;
    }

    if (this->m_sValue == "*")
    {
        return true;
    }

    switch (this->m_type)
    {
        case LindaTupleItemType::Float:
            return stof(this->m_sValue) == lindaTupleItem.GetFloatValue();
        case LindaTupleItemType::Integer:
            return stoi(this->m_sValue) == lindaTupleItem.GetIntegerValue();
        case LindaTupleItemType::String:
            return this->m_sValue == lindaTupleItem.GetStringValue();
        default:
            throw UnknownLindaTupleType();
    }
}


