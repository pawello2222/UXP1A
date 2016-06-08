//
// Created by anowikowski on 27.05.16.
//

#ifndef UXP1A_LINDATUPLEPOOL_H
#define UXP1A_LINDATUPLEPOOL_H


#include <string>
#include "../Model/LindaTuple.h"
#include "../Model/LindaTupleTemplate.h"
#include "../Model/LindaTuplesFileEntry.h"
#include "../Model/LindaWaitingQueueFileEntry.h"
#include "LindaTuplesFileManager.h"
#include "LindaQueueFileManager.h"

class LindaTuplePool
{
public:
    LindaTuplePool();
    ~LindaTuplePool();
    void ConnectPool(std::string tuplesFilePath, std::string waitingQueueFilePath);
    void DisconnectPool();
    LindaTuple Read(LindaTupleTemplate& tupleTemplate, unsigned long timeout);
    LindaTuple Input(LindaTupleTemplate& tupleTemplate, unsigned long timeout);
    void Output(LindaTuple& tuple);
private:
    LindaTuple ReadInputInternal(LindaTupleTemplate& tupleTemplate, unsigned long timeout, bool removeTuple);
    void GuardPoolConnected();

    std::unique_ptr<LindaTuplesFileManager> m_pTuplesFileManager;
    std::unique_ptr<LindaQueueFileManager> m_pQueueFileManager;
    bool m_bIsConnected;

};


#endif //UXP1A_LINDATUPLEPOOL_H
