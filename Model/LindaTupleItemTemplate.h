//
// Created by anowikowski on 28.05.16.
//

#ifndef UXP1A_LINDATUPLEITEMTEMPLATE_H
#define UXP1A_LINDATUPLEITEMTEMPLATE_H


#include <string>
#include "LindaTupleItemType.h"
#include "LindaTuple.h"
#include "LindaTupleItemOperator.h"

class LindaTupleItemTemplate
{
public:
    LindaTupleItemTemplate( LindaTupleItemType _type, LindaTupleItemOperator _operator, std::string _value );
    LindaTupleItemType GetType();
    LindaTupleItemOperator GetOperator();
    std::string GetValue();
    bool IsMatch( LindaTupleItem& lindaTupleItem );
    std::string FormatValueAsFileEntryData();
private:
    bool compareFloat( float f );
    bool compareInteger( int i );
    bool compareString( std::string s );

    LindaTupleItemType m_type;
    LindaTupleItemOperator m_operator;
    std::string m_sValue;
};


#endif //UXP1A_LINDATUPLEITEMTEMPLATE_H
