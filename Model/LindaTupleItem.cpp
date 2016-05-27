//
// Created by anowikowski on 27.05.16.
//

#include "LindaTupleItem.h"
#include "../Exception/UnknownLindaTupleType.h"
#include "../Exception/InvalidOperation.h"

LindaTupleItem::LindaTupleItem(int value) : m_type(LindaTupleItemType::Integer), m_integerValue(value)
{

}

LindaTupleItem::LindaTupleItem(float value) : m_type(LindaTupleItemType::Float), m_floatValue(value)
{

}

LindaTupleItem::LindaTupleItem(std::string value) : m_type(LindaTupleItemType::String), m_stringValue(value)
{

}

LindaTupleItem::LindaTupleItem(const LindaTupleItem &lindaTupleItem) : m_type(lindaTupleItem.m_type)
{
    switch (lindaTupleItem.m_type)
    {
        case LindaTupleItemType::Integer:
            this->m_integerValue = lindaTupleItem.m_integerValue;
            break;
        case LindaTupleItemType::Float:
            this->m_floatValue = lindaTupleItem.m_floatValue;
            break;
        case LindaTupleItemType::String:
            this->m_stringValue = lindaTupleItem.m_stringValue;
            break;
        default:
            throw UnknownLindaTupleType();
    }
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
    return this->m_integerValue;
}

float LindaTupleItem::GetFloatValue()
{
    this->GuardTypeEquals(LindaTupleItemType::Float);
    return this->m_floatValue;
}

std::string LindaTupleItem::GetStringValue()
{
    this->GuardTypeEquals(LindaTupleItemType::String);
    return this->m_stringValue;
}

void LindaTupleItem::GuardTypeEquals(LindaTupleItemType lindaTupleItemType)
{
    if (this->m_type != lindaTupleItemType)
    {
        throw InvalidOperation();
    }
}
