//
// Created by anowikowski on 08.06.16.
//

#include <fcntl.h>
#include <unistd.h>
#include "LindaFileManagerBase.h"
#include "../Exception/FileOperationError.h"

template<typename T>
LindaFileManagerBase<T>::LindaFileManagerBase(std::string filePath)
{
    this->m_iFileDescriptor = open(filePath.c_str(), O_RDWR | O_CREAT, 0666);
    if (this->m_iFileDescriptor == -1)
    {
        throw FileOperationError("Error when opening file.", errno);
    }
}

template<typename T>
LindaFileManagerBase<T>::~LindaFileManagerBase()
{
    close(this->m_iFileDescriptor);
}

template<typename T>
void LindaFileManagerBase<T>::RemoveEntryTakenFlag()
{
    char mask;
    ssize_t bytesRead = this->Read(&mask, 1);
    if (bytesRead != 1)
    {
        throw FileOperationError("Cannot read the linda tuple file entry flag.", errno);
    }

    this->Seek(-1, SEEK_CUR);

    mask = mask & ~this->FileEntryTakenFlagMask;
    ssize_t bytesWritten = this->WriteAndSeekBack(&mask, 1);
    if (bytesWritten != 1)
    {
        throw FileOperationError("Cannot write the linda tuple file entry flag.", errno);
    }
}

template<typename T>
ssize_t LindaFileManagerBase<T>::WriteAndSeekBack(T &obj)
{
    return this->WriteAndSeekBack(reinterpret_cast<char*>(&obj), sizeof(obj));
}

template<typename T>
int LindaFileManagerBase<T>::LockCurrentEntry()
{
    return lockf(this->m_iFileDescriptor, F_LOCK, sizeof(T));
}

template<typename T>
int LindaFileManagerBase<T>::UnlockCurrentEntry()
{
    return lockf(this->m_iFileDescriptor, F_ULOCK, sizeof(T));
}

template<typename T>
void LindaFileManagerBase<T>::Seek(__off_t offset, int whence)
{
    lseek(this->m_iFileDescriptor, offset, whence);
}

template<typename T>
ssize_t LindaFileManagerBase<T>::Read(void *buffer, size_t bytesToRead)
{
    return read(this->m_iFileDescriptor, buffer, bytesToRead);
}

template<typename T>
ssize_t LindaFileManagerBase<T>::WriteAndSeekBack(char *buffer, size_t size)
{
    ssize_t bytesWritten = write(this->m_iFileDescriptor, buffer, size);
    this->Seek(-bytesWritten, SEEK_CUR);
    return bytesWritten;
}







