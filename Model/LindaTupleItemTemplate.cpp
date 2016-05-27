//
// Created by anowikowski on 28.05.16.
//

#include "LindaTupleItemTemplate.h"
#include "../Exception/UnknownLindaTupleType.h"

LindaTupleItemTemplate::LindaTupleItemTemplate(LindaTupleItemType type, std::string value) : m_type(type), m_value(value)
{

}

LindaTupleItemType LindaTupleItemTemplate::GetType()
{
    return this->m_type;
}

std::string LindaTupleItemTemplate::GetValue()
{
    return this->m_value;
}

bool LindaTupleItemTemplate::IsMatch(LindaTupleItem &lindaTupleItem)
{
    if (this->m_type != lindaTupleItem.GetType())
    {
        return false;
    }

    if (this->m_value == "*")
    {
        return true;
    }

    switch (this->m_type)
    {
        case LindaTupleItemType::Float:
            return stof(this->m_value) == lindaTupleItem.GetFloatValue();
        case LindaTupleItemType::Integer:
            return stoi(this->m_value) == lindaTupleItem.GetIntegerValue();
        case LindaTupleItemType::String:
            return this->m_value == lindaTupleItem.GetStringValue();
        default:
            throw UnknownLindaTupleType();
    }
}


