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
