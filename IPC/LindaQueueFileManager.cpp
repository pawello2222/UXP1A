//
// Created by anowikowski on 08.06.16.
//

#include "LindaQueueFileManager.h"
#include "../Exception/LindaTupleMaxSizeExceeded.h"
#include "SemaphoreManager.h"
#include "../ExpressionParser/LindaTemplateParser.h"
#include <cstring>

LindaQueueFileManager::LindaQueueFileManager(std::string filePath) : LindaFileManagerBase(filePath)
{

}

LindaWaitingQueueFileEntry LindaQueueFileManager::CreateFileEntry(LindaTupleTemplate &tupleTemplate)
{
    LindaWaitingQueueFileEntry entry;
    entry.processId = getpid();
    entry.Flags = FileEntryTakenFlagMask;

    std::string tupleTemplateString = tupleTemplate.ToString();
    if (tupleTemplateString.size() > sizeof(entry.TupleTemplateData) - 1)
    {
        throw LindaTupleMaxSizeExceeded();
    }

    strcpy(entry.TupleTemplateData, tupleTemplateString.c_str());
    entry.TupleTemplateData[tupleTemplateString.size()] = this->TupleFileEntryTupleDataEnd;

    return entry;
}

void LindaQueueFileManager::AddMeToWaitingQueueForTemplate(LindaTupleTemplate &tupleTemplate)
{
    this->FindAndLockUnusedFileEntry();
    this->WriteAndSeekBack(tupleTemplate);
    this->UnlockCurrentEntry();
}

void LindaQueueFileManager::RemoveMeFromWaitingQueue()
{
    this->FindAndLockFileEntry([this](const LindaWaitingQueueFileEntry &fileEntry) {
        return ((fileEntry.Flags & this->FileEntryTakenFlagMask) == this->FileEntryTakenFlagMask && fileEntry.processId == getpid());
    }, false, [this](const LindaWaitingQueueFileEntry &fileEntry) {
        this->RemoveEntryTakenFlag();
    });
    this->UnlockCurrentEntry();
}

int LindaQueueFileManager::NotifyProcessesWaitingForTuple(LindaTuple &tuple)
{
    this->FindAndLockFileEntry([this](const LindaWaitingQueueFileEntry &fileEntry) {
        return ((fileEntry.Flags & this->FileEntryTakenFlagMask) == this->FileEntryTakenFlagMask);
    }, false, [this, &tuple](const LindaWaitingQueueFileEntry &fileEntry) {
        LindaTemplateParser templateParser(fileEntry);
        LindaTupleTemplate lindaTupleTemplate = templateParser.parse();
        if (lindaTupleTemplate.IsMatch(tuple)) {
            SemaphoreManager::UnlockSemaphoreWithProcessId(fileEntry.processId);
        }
    });
    this->UnlockCurrentEntry();
}









