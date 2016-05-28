//
// Created by anowikowski on 27.05.16.
//

#ifndef UXP1A_LINDATUPLE_H
#define UXP1A_LINDATUPLE_H


#include <vector>
#include "LindaTupleItem.h"

class LindaTuple final
{
public:
    LindaTuple() = delete;
    LindaTuple(std::vector<LindaTupleItem> lindaTupleItems);
    std::vector<LindaTupleItem>& GetItems();
    std::string ToString();
private:
    std::vector<LindaTupleItem> m_lindaTupleItems;
};


#endif //UXP1A_LINDATUPLE_H
