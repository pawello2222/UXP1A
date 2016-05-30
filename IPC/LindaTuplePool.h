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
    void FindAndLockUnusedEntry();
    LindaTuple ReadAndLock(LindaTupleTemplate &tupleTemplate, int timeout);
    LindaTuplesFileEntry CreateTupleFileEntry(LindaTuple &tuple);

    void RemoveEntryTakenFlag(int fileDescriptor);
    int LockCurrentTupleEntry();
    int UnlockCurrentTupleEntry();
    int LockCurrentQueueEntry();
    int UnlockCurrentQueueEntry();

    LindaWaitingQueueFileEntry CreateWaitingQueueEntry(LindaTupleTemplate &tuple);
    LindaTuple AddToWaitingQueueForTemplate(LindaTupleTemplate& tupleTemplate);
    int NotifyProcessesWaitingForTuple(LindaTuple& tuple);

    int m_iTuplesFd;
    int m_iWaitingQueueFd;
    bool m_bIsConnected;
    static const char FileEntryTakenFlagMask;
    static const char TupleFileEntryTupleDataEnd;
};


#endif //UXP1A_LINDATUPLEPOOL_H
