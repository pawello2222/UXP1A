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
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

const char LindaTuplePool::TupleFileEntryTakenFlagMask = 0b00000001;

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

LindaTuple LindaTuplePool::Read(LindaTupleTemplate& tupleTemplate, int timeout)
{
    this->GuardPoolConnected();
    LindaTuple tuple = this->ReadAndLock(tupleTemplate, timeout);
    this->UnlockCurrentTupleEntry();
    return tuple;
}

LindaTuple LindaTuplePool::Input(LindaTupleTemplate& tupleTemplate, int timeout)
{
    this->GuardPoolConnected();
    LindaTuple tuple = this->ReadAndLock(tupleTemplate, timeout);
    this->RemoveEntryTakenFlag();
    this->UnlockCurrentTupleEntry();
    return tuple;
}

void LindaTuplePool::Output(LindaTuple &tuple)
{
    this->GuardPoolConnected();
    LindaTuplesFileEntry entry = this->CreateTupleFileEntry(tuple);
    this->FindAndLockUnusedEntry();

    write(this->m_iTuplesFd, reinterpret_cast<char*>(&entry), sizeof(entry));
    //TODO Wake process that is waiting for such a tuple
}

void LindaTuplePool::GuardPoolConnected()
{
    if (!this->m_bIsConnected)
    {
        throw InvalidOperation();
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
        else if ((fileEntry.Flags & this->TupleFileEntryTakenFlagMask) != this->TupleFileEntryTakenFlagMask)
        {
            break;
        }

        //Remove lock
        this->UnlockCurrentTupleEntry();

        //Seek forward to beginning of next tuple
        lseek(this->m_iTuplesFd, sizeof(LindaTuplesFileEntry),  SEEK_CUR);
    } while(true);
}

LindaTuple LindaTuplePool::ReadAndLock(LindaTupleTemplate &tupleTemplate, int timeout)
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
        else if ((fileEntry.Flags & this->TupleFileEntryTakenFlagMask) == this->TupleFileEntryTakenFlagMask)
        {
            //Entry is taken, checking if it matches template
            //TODO Integrate with Parser (Uncomment the following)
            /*LindaTuple tuple(fileEntry);
            if (tupleTemplate.IsMatch(tuple))
            {
                return tuple;
            }*/
        }

        //Remove lock
        this->UnlockCurrentTupleEntry();

        //Seek forward to beginning of next tuple
        lseek(this->m_iTuplesFd, sizeof(LindaTuplesFileEntry),  SEEK_CUR);
    } while(true);

    //TODO If not found add self to waiting queue
    throw std::logic_error("Not implemented exception");
}

void LindaTuplePool::RemoveEntryTakenFlag()
{
    char mask;
    ssize_t bytesRead = read(this->m_iTuplesFd, &mask, 1);
    if (bytesRead != 1)
    {
        throw FileOperationError("Cannot read the linda tuple file entry flag.", errno);
    }

    lseek(this->m_iTuplesFd, -1, SEEK_CUR);

    mask = mask & ~this->TupleFileEntryTakenFlagMask;
    ssize_t bytesWritten = write(this->m_iTuplesFd, &mask, 1);
    if (bytesWritten != 1)
    {
        throw FileOperationError("Cannot write the linda tuple file entry flag.", errno);
    }

    lseek(this->m_iTuplesFd, -1, SEEK_CUR);
}

int LindaTuplePool::UnlockCurrentTupleEntry()
{
    return lockf(this->m_iTuplesFd, F_ULOCK, sizeof(LindaTuplesFileEntry));
}

int LindaTuplePool::LockCurrentTupleEntry()
{
    return lockf(this->m_iTuplesFd, F_LOCK, sizeof(LindaTuplesFileEntry));
}


