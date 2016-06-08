//
// Created by anowikowski on 08.06.16.
//

#ifndef UXP1A_LINDAQUEUEFILEMANAGER_H
#define UXP1A_LINDAQUEUEFILEMANAGER_H


#include "LindaFileManagerBase.h"
#include "../Model/LindaTupleTemplate.h"
#include "../Model/LindaWaitingQueueFileEntry.h"

class LindaQueueFileManager : public LindaFileManagerBase<LindaWaitingQueueFileEntry, LindaTupleTemplate>
{
public:
    LindaQueueFileManager(std::string filePath);
    void AddMeToWaitingQueueForTemplate(LindaTupleTemplate& tupleTemplate);
    void RemoveMeFromWaitingQueue();
    int NotifyProcessesWaitingForTuple(LindaTuple& tuple);
private:
    LindaWaitingQueueFileEntry CreateFileEntry(LindaTupleTemplate &tupleTemplate);
};


#endif //UXP1A_LINDAQUEUEFILEMANAGER_H
