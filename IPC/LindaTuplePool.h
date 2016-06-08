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
    LindaTuple Read(LindaTupleTemplate& tupleTemplate, unsigned long timeout);
    LindaTuple Input(LindaTupleTemplate& tupleTemplate, unsigned long timeout);
    void Output(LindaTuple& tuple);
private:
    LindaTuple ReadInputInternal(LindaTupleTemplate& tupleTemplate, unsigned long timeout, bool removeTuple);
    void GuardPoolConnected();
    void FindAndLockUnusedTupleEntry();
    LindaTuple ReadAndLock(LindaTupleTemplate &tupleTemplate);
    LindaTuplesFileEntry CreateTupleFileEntry(LindaTuple &tuple);

    void RemoveEntryTakenFlag(int fileDescriptor);
    int LockCurrentTupleEntry();
    int UnlockCurrentTupleEntry();
    int LockCurrentQueueEntry();
    int UnlockCurrentQueueEntry();
    ssize_t WriteAndSeekBack(int fileDescriptor, char* buffer, size_t size);

    LindaWaitingQueueFileEntry CreateWaitingQueueEntry(LindaTupleTemplate &tuple);
    void AddMeToWaitingQueueForTemplate(LindaTupleTemplate& tupleTemplate);
    void RemoveMeFromWaitingQueue();
    int NotifyProcessesWaitingForTuple(LindaTuple& tuple);

    int m_iTuplesFd;
    int m_iWaitingQueueFd;
    bool m_bIsConnected;
    static const char FileEntryTakenFlagMask;
    static const char TupleFileEntryTupleDataEnd;
};


#endif //UXP1A_LINDATUPLEPOOL_H
