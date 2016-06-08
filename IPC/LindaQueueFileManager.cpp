//
// Created by anowikowski on 08.06.16.
//

#include "LindaQueueFileManager.h"
#include "../Exception/LindaTupleMaxSizeExceeded.h"
#include "../Exception/LindaFileCorrupt.h"
#include "../Exception/LockingError.h"
#include "../Exception/FileOperationError.h"
#include "SemaphoreManager.h"
#include "../ExpressionParser/LindaTemplateParser.h"
#include <unistd.h>
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
    //Seek to file begin
    this->Seek(0, SEEK_SET);

    LindaWaitingQueueFileEntry fileEntry;
    do
    {
        //Lock tuple file entry
        if (this->LockCurrentEntry() != 0)
        {
            throw LockingError("Couldn't set lock on tuple entry.", errno);
        }

        //Read tuple file entry
        ssize_t result = this->Read(&fileEntry, sizeof(fileEntry));
        if (result == -1)
        {
            this->UnlockCurrentEntry();
            throw FileOperationError("Error reading tuple from file.", errno);
        }

        //Seek back to beginning of tuple
        this->Seek(-result, SEEK_CUR);

        ssize_t bytesRead = result;
        if (bytesRead == 0)
        {
            //End of file reached
            break;
        }
        if (bytesRead != sizeof(fileEntry))
        {
            //This should not happen
            this->UnlockCurrentEntry();
            throw LindaFileCorrupt();
        }
        else if ((fileEntry.Flags & this->FileEntryTakenFlagMask) != this->FileEntryTakenFlagMask)
        {
            break;
        }

        //Remove lock
        this->UnlockCurrentEntry();

        //Seek forward to beginning of next tuple
        this->Seek(sizeof(fileEntry),  SEEK_CUR);
    } while(true);

    this->WriteAndSeekBack(tupleTemplate);
    this->UnlockCurrentEntry();
}

void LindaQueueFileManager::RemoveMeFromWaitingQueue()
{
    this->Seek(0, SEEK_SET);
    LindaWaitingQueueFileEntry fileEntry;
    do
    {
        //Lock queue file entry
        if (this->LockCurrentEntry()!= 0)
        {
            throw LockingError("Couldn't set lock on queue entry.", errno);
        }

        //Read queue file entry
        ssize_t result = this->Read(&fileEntry, sizeof(fileEntry));
        if (result == -1)
        {
            this->UnlockCurrentEntry();
            throw FileOperationError("Error reading queue entry from file.", errno);
        }

        //Seek back to beginning of tuple
        this->Seek(-result, SEEK_CUR);

        ssize_t bytesRead = result;
        if (bytesRead == 0)
        {
            //End of file reached
            this->UnlockCurrentEntry();
            break;
        }
        if (bytesRead != sizeof(fileEntry))
        {
            //This should not happen
            this->UnlockCurrentEntry();
            throw LindaFileCorrupt();
        }
        else if ((fileEntry.Flags & this->FileEntryTakenFlagMask) == this->FileEntryTakenFlagMask && fileEntry.processId == getpid())
        {
            //This is my entry
            this->RemoveEntryTakenFlag();
        }

        //Remove lock
        this->UnlockCurrentEntry();

        //Seek forward to beginning of next tuple
        this->Seek(sizeof(LindaWaitingQueueFileEntry),  SEEK_CUR);
    } while(true);
}

int LindaQueueFileManager::NotifyProcessesWaitingForTuple(LindaTuple &tuple)
{
    int notifiedProcessesCount = 0;
    this->Seek(0, SEEK_SET);
    LindaWaitingQueueFileEntry fileEntry;
    do
    {
        //Lock tuple file entry
        if (this->LockCurrentEntry()!= 0)
        {
            throw LockingError("Couldn't set lock on queue entry.", errno);
        }

        //Read tuple file entry
        ssize_t result = this->Read(&fileEntry, sizeof(fileEntry));
        if (result == -1)
        {
            this->UnlockCurrentEntry();
            throw FileOperationError("Error reading queue entry from file.", errno);
        }

        //Seek back to beginning of tuple
        this->Seek(-result, SEEK_CUR);

        ssize_t bytesRead = result;
        if (bytesRead == 0)
        {
            //End of file reached
            this->UnlockCurrentEntry();
            break;
        }
        if (bytesRead != sizeof(fileEntry))
        {
            //This should not happen
            this->UnlockCurrentEntry();
            throw LindaFileCorrupt();
        }
        else if ((fileEntry.Flags & this->FileEntryTakenFlagMask) == this->FileEntryTakenFlagMask)
        {
            //Entry is taken, checking if it matches template
            LindaTemplateParser templateParser(fileEntry);
            LindaTupleTemplate lindaTupleTemplate = templateParser.parse();
            if (lindaTupleTemplate.IsMatch(tuple)) {
                notifiedProcessesCount++;
                SemaphoreManager::UnlockSemaphoreWithProcessId(fileEntry.processId);
            }
        }

        //Remove lock
        this->UnlockCurrentEntry();

        //Seek forward to beginning of next tuple
        this->Seek(sizeof(LindaWaitingQueueFileEntry),  SEEK_CUR);
    } while(true);

    return notifiedProcessesCount;
}









