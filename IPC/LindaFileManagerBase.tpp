//
// Created by anowikowski on 08.06.16.
//

#include <fcntl.h>
#include <unistd.h>
#include "LindaFileManagerBase.h"
#include "../Exception/FileOperationError.h"
#include "../Exception/LockingError.h"
#include "../Exception/LindaFileCorrupt.h"

template<typename T1, typename T2>
LindaFileManagerBase<T1, T2>::LindaFileManagerBase(std::string filePath)
{
    this->m_iFileDescriptor = open(filePath.c_str(), O_RDWR | O_CREAT, 0666);
    if (this->m_iFileDescriptor == -1)
    {
        throw FileOperationError("Error when opening file.", errno);
    }
}

template<typename T1, typename T2>
LindaFileManagerBase<T1, T2>::~LindaFileManagerBase()
{
    close(this->m_iFileDescriptor);
}

template<typename T1, typename T2>
void LindaFileManagerBase<T1, T2>::RemoveEntryTakenFlag()
{
    char mask;
    ssize_t bytesRead = this->ReadAndSeekBack(&mask, 1);
    if (bytesRead != 1)
    {
        throw FileOperationError("Cannot read the linda tuple file entry flag.", errno);
    }

    mask = mask & ~this->FileEntryTakenFlagMask;
    ssize_t bytesWritten = this->WriteAndSeekBack(&mask, 1);
    if (bytesWritten != 1)
    {
        throw FileOperationError("Cannot write the linda tuple file entry flag.", errno);
    }
}

template<typename T1, typename T2>
ssize_t LindaFileManagerBase<T1, T2>::WriteFileEntryAndSeekBack(T1 &fileEntry)
{
    return this->WriteAndSeekBack(reinterpret_cast<char*>(&fileEntry), sizeof(fileEntry));
}

template<typename T1, typename T2>
ssize_t LindaFileManagerBase<T1, T2>::WriteAndSeekBack(T2 &obj)
{
    T1 fileEntry = this->CreateFileEntry(obj);
    return this->WriteFileEntryAndSeekBack(fileEntry);
}

template<typename T1, typename T2>
int LindaFileManagerBase<T1, T2>::LockCurrentEntry()
{
    return lockf(this->m_iFileDescriptor, F_LOCK, sizeof(T1));
}

template<typename T1, typename T2>
int LindaFileManagerBase<T1, T2>::UnlockCurrentEntry()
{
    return lockf(this->m_iFileDescriptor, F_ULOCK, sizeof(T1));
}

template<typename T1, typename T2>
void LindaFileManagerBase<T1, T2>::Seek(__off_t offset, int whence)
{
    lseek(this->m_iFileDescriptor, offset, whence);
}

template<typename T1, typename T2>
ssize_t LindaFileManagerBase<T1, T2>::ReadAndSeekBack(void *buffer, size_t bytesToRead)
{
    ssize_t bytesRead = read(this->m_iFileDescriptor, buffer, bytesToRead);
    if (bytesRead > 0) {
        this->Seek(-bytesRead, SEEK_CUR);
    }

    return bytesRead;
}

template<typename T1, typename T2>
ssize_t LindaFileManagerBase<T1, T2>::WriteAndSeekBack(char *buffer, size_t size)
{
    ssize_t bytesWritten = write(this->m_iFileDescriptor, buffer, size);
    this->Seek(-bytesWritten, SEEK_CUR);
    return bytesWritten;
}

template<typename T1, typename T2>
bool LindaFileManagerBase<T1, T2>::FindAndLockFileEntry(std::function<bool(T1)> predicate, bool processOnlyFirstMatchingPredicate, std::function<void(T1 foundFileEntry)> onFound)
{
    bool found = false;
    this->Seek(0, SEEK_SET);
    T1 fileEntry;
    do
    {
        //Lock file entry
        if (this->LockCurrentEntry() != 0)
        {
            throw LockingError("Couldn't set lock on entry.", errno);
        }

        //Read file entry
        ssize_t result = this->ReadAndSeekBack(&fileEntry, sizeof(fileEntry));
        if (result == -1)
        {
            this->UnlockCurrentEntry();
            throw FileOperationError("Error reading entry from file.", errno);
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
            this->UnlockCurrentEntry();
            throw LindaFileCorrupt();
        }
        else if (predicate(fileEntry))
        {
            found = true;
            onFound(fileEntry);
            if (processOnlyFirstMatchingPredicate) {
                break;
            }
        }

        //Remove lock
        this->UnlockCurrentEntry();

        //Seek forward to beginning of next tuple
        this->Seek(sizeof(fileEntry),  SEEK_CUR);
    } while(true);

    return found;
}

template<typename T1, typename T2>
void LindaFileManagerBase<T1, T2>::FindAndLockUnusedFileEntry()
{
    this->FindAndLockFileEntry([this](const T1 &fileEntry)
                                      {
                                          return ((fileEntry.Flags & this->FileEntryTakenFlagMask) !=
                                                  this->FileEntryTakenFlagMask);
                                      }, true);
}











