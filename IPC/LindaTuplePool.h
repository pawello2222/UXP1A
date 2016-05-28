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
    LindaTuplePool();
    ~LindaTuplePool();
    void ConnectPool(std::string tuplesFilePath, std::string waitingQueueFilePath);
    void DisconnectPool();
    LindaTuple Read(LindaTupleTemplate& tupleTemplate, int timeout);
    LindaTuple Input(LindaTupleTemplate& tupleTemplate, int timeout);
    void Output(LindaTuple& tuple);
private:
    void GuardPoolConnected();
    int m_iTuplesFd;
    int m_iWaitingQueueFd;
    bool m_bIsConnected;
};


#endif //UXP1A_LINDATUPLEPOOL_H
