//
// Created by anowikowski on 28.05.16.
//

#include "LindaTupleItemTemplate.h"
#include "../Exception/UnknownLindaTupleType.h"
#include "../Exception/UnknownLindaTupleOperator.h"

LindaTupleItemTemplate::LindaTupleItemTemplate( LindaTupleItemType _type, LindaTupleItemOperator _operator, std::string _value )
        : m_type( _type ), m_operator( _operator ), m_sValue( _value )
{

}

LindaTupleItemType LindaTupleItemTemplate::GetType()
{
    return this->m_type;
}

LindaTupleItemOperator LindaTupleItemTemplate::GetOperator()
{
    return this->m_operator;
}

std::string LindaTupleItemTemplate::GetValue()
{
    return this->m_sValue;
}

bool LindaTupleItemTemplate::IsMatch( LindaTupleItem& lindaTupleItem )
{
    if ( this->m_type != lindaTupleItem.GetType() )
    {
        return false;
    }

    if ( this->m_sValue == "*" )
    {
        return true;
    }

    switch ( this->m_type )
    {
        case LindaTupleItemType::Float:
            return compareFloat( lindaTupleItem.GetFloatValue() );
        case LindaTupleItemType::Integer:
            return compareInteger( lindaTupleItem.GetIntegerValue() );
        case LindaTupleItemType::String:
            return compareString( lindaTupleItem.GetStringValue() );
        default:
            throw UnknownLindaTupleType();
    }
}

bool LindaTupleItemTemplate::compareFloat( float f )
{
    switch( this->m_operator )
    {
        case lt:
            return stoi( this->m_sValue ) > f;
        case gt:
            return stoi( this->m_sValue ) < f;
        default:
            throw UnknownLindaTupleOperator();
    }
}

bool LindaTupleItemTemplate::compareInteger( int i )
{
    switch( this->m_operator )
    {
        case lt:
            return stoi( this->m_sValue ) > i;
        case le:
            return stoi( this->m_sValue ) >= i;
        case eq:
            return stoi( this->m_sValue ) == i;
        case ge:
            return stoi( this->m_sValue ) <= i;
        case gt:
            return stoi( this->m_sValue ) < i;
        default:
            throw UnknownLindaTupleOperator();
    }
}

bool LindaTupleItemTemplate::compareString( std::string s )
{
    switch( this->m_operator )
    {
        case lt:
            return this->m_sValue > s;
        case le:
            return this->m_sValue >= s;
        case eq:
            return this->m_sValue == s;
        case ge:
            return this->m_sValue <= s;
        case gt:
            return this->m_sValue < s;
        default:
            throw UnknownLindaTupleOperator();
    }
}
