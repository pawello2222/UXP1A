//
// Created by anowikowski on 08.06.16.
//

#ifndef UXP1A_LINDAQUEUEFILEMANAGER_H
#define UXP1A_LINDAQUEUEFILEMANAGER_H


#include "LindaFileManagerBase.h"
#include "../Model/LindaTupleTemplate.h"
#include "../Model/LindaWaitingQueueFileEntry.h"

class LindaQueueFileManager : public LindaFileManagerBase<LindaWaitingQueueFileEntry>
{
public:
    LindaQueueFileManager(std::string filePath);
    LindaWaitingQueueFileEntry CreateWaitingQueueEntry(LindaTupleTemplate &tupleTemplate);
    void AddMeToWaitingQueueForTemplate(LindaTupleTemplate& tupleTemplate);
    void RemoveMeFromWaitingQueue();
    int NotifyProcessesWaitingForTuple(LindaTuple& tuple);
};


#endif //UXP1A_LINDAQUEUEFILEMANAGER_H
