//
// Created by anowikowski on 27.05.16.
//

#ifndef UXP1A_LINDATUPLEITEM_H
#define UXP1A_LINDATUPLEITEM_H


#include <string>
#include <memory>
#include "LindaTupleItemType.h"

class LindaTupleItem final
{
public:
    LindaTupleItem() = delete;
    LindaTupleItem(int value);
    LindaTupleItem(float value);
    LindaTupleItem(std::string value);

    ~LindaTupleItem();

    LindaTupleItemType GetType();
    int GetIntegerValue();
    float GetFloatValue();
    std::string GetStringValue();
private:
    void GuardTypeEquals(LindaTupleItemType lindaTupleItemType);
    LindaTupleItemType m_type;
    int m_integerValue;
    float m_floatValue;
    std::string m_stringValue;
};


#endif //UXP1A_LINDATUPLEITEM_H
