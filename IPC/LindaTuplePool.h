//
// Created by anowikowski on 27.05.16.
//

#ifndef UXP1A_LINDATUPLEPOOL_H
#define UXP1A_LINDATUPLEPOOL_H


#include <string>
#include "../Model/LindaTuple.h"
#include "../Model/LindaTupleTemplate.h"

class LindaTuplePool
{
public:
    LindaTuplePool(std::string tuplesFilePath, std::string waitingQueueFilePath);
    LindaTuple Read(LindaTupleTemplate& tupleTemplate, int timeout);
    LindaTuple Input(LindaTupleTemplate& tupleTemplate, int timeout);
    void Output(LindaTuple& tuple);
};


#endif //UXP1A_LINDATUPLEPOOL_H
