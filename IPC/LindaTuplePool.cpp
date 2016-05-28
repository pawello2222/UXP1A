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

const int LindaTuplePool::TupleFileEntryTakenFlagMask = 0b00000001;

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
    //TODO
}

LindaTuple LindaTuplePool::Input(LindaTupleTemplate& tupleTemplate, int timeout)
{
    this->GuardPoolConnected();
    //TODO
}

void LindaTuplePool::Output(LindaTuple &tuple)
{
    this->GuardPoolConnected();
    LindaTuplesFileEntry entry = this->CreateTupleFileEntry(tuple);
    this->FindAndLockUnusedEntry();

    write(this->m_iTuplesFd, reinterpret_cast<char*>(&entry), sizeof(entry));
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
    entry.TupleData[tupleString.size()] = '\x00';

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
        if (lockf(this->m_iTuplesFd, F_LOCK, sizeof(LindaTuplesFileEntry)) != 0)
        {
            throw LockingError("Couldn't set lock on tuple entry.", errno);
        }

        //Read tuple file entry
        ssize_t result = read(this->m_iTuplesFd, &fileEntry, sizeof(fileEntry));
        if (result == -1)
        {
            lockf(this->m_iTuplesFd, F_ULOCK, sizeof(LindaTuplesFileEntry));
            throw FileOperationError("Error reading tuple from file.", errno);
        }

        ssize_t bytesRead = result;
        if (bytesRead == 0)
        {
            //End of file reached
            break;
        }
        if (bytesRead != sizeof(fileEntry))
        {
            //This should not happen
            lockf(this->m_iTuplesFd, F_ULOCK, sizeof(LindaTuplesFileEntry));
            throw LindaFileCorrupt();
        }
        else if ((fileEntry.Flags & this->TupleFileEntryTakenFlagMask) == 0)
        {
            //Seek back to beginning of tuple file entry
            lseek(this->m_iTuplesFd, -result, SEEK_CUR);
            break;
        }

        //Remove lock
        lockf(this->m_iTuplesFd, F_ULOCK, sizeof(LindaTuplesFileEntry));
    } while(true);
}
