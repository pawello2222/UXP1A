//
// Created by anowikowski on 27.05.16.
//

#include <iostream>
#include "LindaTupleItem.h"
#include "../Exception/InvalidOperation.h"

LindaTupleItem::LindaTupleItem(int value) : m_type(LindaTupleItemType::Integer), m_iValue(value)
{

}

LindaTupleItem::LindaTupleItem(float value) : m_type(LindaTupleItemType::Float), m_fValue(value)
{

}

LindaTupleItem::LindaTupleItem(std::string value) : m_type(LindaTupleItemType::String), m_sValue(value)
{

}

LindaTupleItem::~LindaTupleItem()
{

}

LindaTupleItemType LindaTupleItem::GetType()
{
    return this->m_type;
}

int LindaTupleItem::GetIntegerValue()
{
    this->GuardTypeEquals(LindaTupleItemType::Integer);
    return this->m_iValue;
}

float LindaTupleItem::GetFloatValue()
{
    this->GuardTypeEquals(LindaTupleItemType::Float);
    return this->m_fValue;
}

std::string LindaTupleItem::GetStringValue()
{
    this->GuardTypeEquals(LindaTupleItemType::String);
    return this->m_sValue;
}

void LindaTupleItem::GuardTypeEquals(LindaTupleItemType lindaTupleItemType)
{
    if (this->m_type != lindaTupleItemType)
    {
        throw InvalidOperation();
    }
}
