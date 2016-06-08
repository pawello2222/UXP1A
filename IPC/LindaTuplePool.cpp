//
// Created by anowikowski on 27.05.16.
//

#include "LindaTuplePool.h"
#include "../Exception/LindaTuplePoolConnectionError.h"
#include "../Exception/InvalidOperation.h"
#include "../Exception/LindaTupleMaxSizeExceeded.h"
#include "../Exception/LockingError.h"
#include "../Exception/LindaFileCorrupt.h"
#include "../Exception/FileOperationError.h"
#include "../ExpressionParser/LindaTupleParser.h"
#include "../Model/LindaWaitingQueueFileEntry.h"
#include "SemaphoreManager.h"
#include "../ExpressionParser/LindaTemplateParser.h"
#include "../Exception/LindaPoolOperationTimedOutException.h"
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>

const char LindaTuplePool::FileEntryTakenFlagMask = 0b00000001;

const char LindaTuplePool::TupleFileEntryTupleDataEnd = '\x00';

LindaTuplePool::LindaTuplePool() : m_bIsConnected(false)
{

}

LindaTuplePool::~LindaTuplePool()
{
    if (this->m_bIsConnected)
    {
        this->DisconnectPool();
    }
}

void LindaTuplePool::ConnectPool(std::string lindaTuplesFilePath, std::string lindaWaitingQueueFilePath)
{
    this->m_iTuplesFd = open(lindaTuplesFilePath.c_str(), O_RDWR | O_CREAT, 0666);
    if (this->m_iTuplesFd == -1)
    {
        throw LindaTuplePoolConnectionError("Error when opening tuples file.", errno);
    }

    this->m_iWaitingQueueFd = open(lindaWaitingQueueFilePath.c_str(), O_RDWR | O_CREAT, 0666);
    if (this->m_iWaitingQueueFd == -1)
    {
        close(this->m_iTuplesFd);
        throw LindaTuplePoolConnectionError("Error when opening waiting queue file.", errno);
    }

    this->m_bIsConnected = true;
}

void LindaTuplePool::DisconnectPool()
{
    if (this->m_bIsConnected)
    {
        close(this->m_iTuplesFd);
        close(this->m_iWaitingQueueFd);
        this->m_bIsConnected = false;
    }
}

LindaTuple LindaTuplePool::Read(LindaTupleTemplate& tupleTemplate, unsigned long timeout)
{
    this->GuardPoolConnected();
    LindaTuple tuple = this->ReadAndLock(tupleTemplate, timeout);
    this->UnlockCurrentTupleEntry();
    return tuple;
}

LindaTuple LindaTuplePool::Input(LindaTupleTemplate& tupleTemplate, unsigned long timeout)
{
    this->GuardPoolConnected();
    LindaTuple tuple = this->ReadAndLock(tupleTemplate, timeout);
    this->RemoveEntryTakenFlag(this->m_iTuplesFd);
    this->UnlockCurrentTupleEntry();
    return tuple;
}

void LindaTuplePool::Output(LindaTuple &tuple)
{
    this->GuardPoolConnected();
    LindaTuplesFileEntry entry = this->CreateTupleFileEntry(tuple);
    this->FindAndLockUnusedEntry();

    write(this->m_iTuplesFd, reinterpret_cast<char*>(&entry), sizeof(entry));
    //TODO: Maybe unlock?
    NotifyProcessesWaitingForTuple(tuple);
}

void LindaTuplePool::GuardPoolConnected()
{
    if (!this->m_bIsConnected)
    {
        throw InvalidOperation("Not connected to the pool.");
    }
}

LindaTuplesFileEntry LindaTuplePool::CreateTupleFileEntry(LindaTuple &tuple)
{
    LindaTuplesFileEntry entry;
    entry.Flags = 1;

    std::string tupleString = tuple.ToString();
    if (tupleString.size() > sizeof(entry.TupleData) - 1)
    {
        throw LindaTupleMaxSizeExceeded();
    }

    strcpy(entry.TupleData, tupleString.c_str());
    entry.TupleData[tupleString.size()] = this->TupleFileEntryTupleDataEnd;

    return entry;
}

void LindaTuplePool::FindAndLockUnusedEntry()
{
    //Seek to file begin
    lseek(this->m_iTuplesFd, 0, SEEK_SET);

    LindaTuplesFileEntry fileEntry;
    do
    {
        //Lock tuple file entry
        if (this->LockCurrentTupleEntry() != 0)
        {
            throw LockingError("Couldn't set lock on tuple entry.", errno);
        }

        //Read tuple file entry
        ssize_t result = read(this->m_iTuplesFd, &fileEntry, sizeof(fileEntry));
        if (result == -1)
        {
            this->UnlockCurrentTupleEntry();
            throw FileOperationError("Error reading tuple from file.", errno);
        }

        //Seek back to beginning of tuple
        lseek(this->m_iTuplesFd, -result, SEEK_CUR);

        ssize_t bytesRead = result;
        if (bytesRead == 0)
        {
            //End of file reached
            break;
        }
        if (bytesRead != sizeof(fileEntry))
        {
            //This should not happen
            this->UnlockCurrentTupleEntry();
            throw LindaFileCorrupt();
        }
        else if ((fileEntry.Flags & this->FileEntryTakenFlagMask) != this->FileEntryTakenFlagMask)
        {
            break;
        }

        //Remove lock
        this->UnlockCurrentTupleEntry();

        //Seek forward to beginning of next tuple
        lseek(this->m_iTuplesFd, sizeof(LindaTuplesFileEntry),  SEEK_CUR);
    } while(true);
}

LindaTuple LindaTuplePool::ReadAndLock(LindaTupleTemplate &tupleTemplate, unsigned long timeout)
{
    //Seek to file begin
    lseek(this->m_iTuplesFd, 0, SEEK_SET);

    LindaTuplesFileEntry fileEntry;
    do
    {
        //Lock tuple file entry
        if (this->LockCurrentTupleEntry() != 0)
        {
            throw LockingError("Couldn't set lock on tuple entry.", errno);
        }

        //Read tuple file entry
        ssize_t result = read(this->m_iTuplesFd, &fileEntry, sizeof(fileEntry));
        if (result == -1)
        {
            this->UnlockCurrentTupleEntry();
            throw FileOperationError("Error reading tuple from file.", errno);
        }

        //Seek back to beginning of tuple
        lseek(this->m_iTuplesFd, -result, SEEK_CUR);

        ssize_t bytesRead = result;
        if (bytesRead == 0)
        {
            //End of file reached
            break;
        }
        if (bytesRead != sizeof(fileEntry))
        {
            //This should not happen
            this->UnlockCurrentTupleEntry();
            throw LindaFileCorrupt();
        }
        else if ((fileEntry.Flags & this->FileEntryTakenFlagMask) == this->FileEntryTakenFlagMask)
        {
            //Entry is taken, checking if it matches template
            LindaTupleParser parser(fileEntry);
            LindaTuple tuple = parser.parse();
            if (tupleTemplate.IsMatch(tuple))
            {
                return tuple;
            }
        }

        //Remove lock
        this->UnlockCurrentTupleEntry();

        //Seek forward to beginning of next tuple
        lseek(this->m_iTuplesFd, sizeof(LindaTuplesFileEntry),  SEEK_CUR);
    } while(true);

    //Wait for template until someone will insert it.
    return AddToWaitingQueueForTemplate(tupleTemplate, timeout);
}

void LindaTuplePool::RemoveEntryTakenFlag(int fileDescriptor)
{
    char mask;
    ssize_t bytesRead = read(fileDescriptor, &mask, 1);
    if (bytesRead != 1)
    {
        throw FileOperationError("Cannot read the linda tuple file entry flag.", errno);
    }

    lseek(fileDescriptor, -1, SEEK_CUR);

    mask = mask & ~this->FileEntryTakenFlagMask;
    ssize_t bytesWritten = write(fileDescriptor, &mask, 1);
    if (bytesWritten != 1)
    {
        throw FileOperationError("Cannot write the linda tuple file entry flag.", errno);
    }

    lseek(fileDescriptor, -1, SEEK_CUR);
}

int LindaTuplePool::UnlockCurrentTupleEntry()
{
    return lockf(this->m_iTuplesFd, F_ULOCK, sizeof(LindaTuplesFileEntry));
}

int LindaTuplePool::LockCurrentTupleEntry()
{
    return lockf(this->m_iTuplesFd, F_LOCK, sizeof(LindaTuplesFileEntry));
}


//QUEUE operation sections
int LindaTuplePool::UnlockCurrentQueueEntry() {
    return lockf(this->m_iWaitingQueueFd, F_ULOCK, sizeof(LindaWaitingQueueFileEntry));
}

int LindaTuplePool::LockCurrentQueueEntry() {
    return lockf(this->m_iWaitingQueueFd, F_LOCK, sizeof(LindaWaitingQueueFileEntry));
}



//TODO: Methods reading and sharing entries should get refactor - Kacper


LindaWaitingQueueFileEntry LindaTuplePool::CreateWaitingQueueEntry(LindaTupleTemplate &tuple) {
    //TODO: Create this method
    throw "not implemented yet";
}

//Blocking operation
LindaTuple LindaTuplePool::AddToWaitingQueueForTemplate(LindaTupleTemplate &tupleTemplate, unsigned long timeout) {
    //Seek to file begin
    lseek(this->m_iTuplesFd, 0, SEEK_SET);

    LindaWaitingQueueFileEntry fileEntry;
    do
    {
        //Lock tuple file entry
        if (this->LockCurrentQueueEntry() != 0)
        {
            throw LockingError("Couldn't set lock on tuple entry.", errno);
        }

        //Read tuple file entry
        ssize_t result = read(this->m_iWaitingQueueFd, &fileEntry, sizeof(fileEntry));
        if (result == -1)
        {
            this->UnlockCurrentTupleEntry();
            throw FileOperationError("Error reading tuple from file.", errno);
        }

        //Seek back to beginning of tuple
        lseek(this->m_iWaitingQueueFd, -result, SEEK_CUR);

        ssize_t bytesRead = result;
        if (bytesRead == 0)
        {
            //End of file reached
            break;
        }
        if (bytesRead != sizeof(fileEntry))
        {
            //This should not happen
            this->UnlockCurrentQueueEntry();
            throw LindaFileCorrupt();
        }
        else if ((fileEntry.Flags & this->FileEntryTakenFlagMask) != this->FileEntryTakenFlagMask)
        {
            break;
        }

        //Remove lock
        this->UnlockCurrentQueueEntry();

        //Seek forward to beginning of next tuple
        lseek(this->m_iWaitingQueueFd, sizeof(fileEntry),  SEEK_CUR);
    } while(true);

    LindaWaitingQueueFileEntry entry = CreateWaitingQueueEntry(tupleTemplate);
    write(this->m_iWaitingQueueFd, reinterpret_cast<char*>(&entry), sizeof(entry));
    UnlockCurrentQueueEntry();
    auto result = SemaphoreManager::LockOnSemaphoreWithTimeout(timeout);
    if (result == -1) {
        throw LindaPoolOperationTimedOutException();
    }
    else {
        //TODO: Now we should read entry from given line.
    }


}

int LindaTuplePool::NotifyProcessesWaitingForTuple(LindaTuple &tuple) {
    int notifiedProcessesCount = 0;
    lseek(this->m_iWaitingQueueFd, 0, SEEK_SET);
    LindaWaitingQueueFileEntry fileEntry;
    do
    {
        //Lock tuple file entry
        if (this->LockCurrentQueueEntry()!= 0)
        {
            throw LockingError("Couldn't set lock on queue entry.", errno);
        }

        //Read tuple file entry
        ssize_t result = read(this->m_iWaitingQueueFd, &fileEntry, sizeof(fileEntry));
        if (result == -1)
        {
            this->UnlockCurrentQueueEntry();
            throw FileOperationError("Error reading queue entry from file.", errno);
        }

        //Seek back to beginning of tuple
        lseek(this->m_iWaitingQueueFd, -result, SEEK_CUR);

        ssize_t bytesRead = result;
        if (bytesRead == 0)
        {
            //End of file reached
            break;
        }
        if (bytesRead != sizeof(fileEntry))
        {
            //This should not happen
            this->UnlockCurrentQueueEntry();
            throw LindaFileCorrupt();
        }
        else if ((fileEntry.Flags & this->FileEntryTakenFlagMask) == this->FileEntryTakenFlagMask)
        {
            //Entry is taken, checking if it matches template
            LindaTemplateParser templateParser(fileEntry);
            LindaTupleTemplate lindaTupleTemplate = templateParser.parse();
            if (lindaTupleTemplate.IsMatch(tuple)) {
                notifiedProcessesCount++;
                //TODO: We should let know, where this tuple is
                SemaphoreManager::UnlockSemaphoreWithProcessId(fileEntry.processId);
                this->RemoveEntryTakenFlag(this->m_iWaitingQueueFd);
            }
        }

        //Remove lock
        this->UnlockCurrentQueueEntry();

        //Seek forward to beginning of next tuple
        lseek(this->m_iWaitingQueueFd, sizeof(LindaWaitingQueueFileEntry),  SEEK_CUR);
    } while(true);

    return notifiedProcessesCount;
}

