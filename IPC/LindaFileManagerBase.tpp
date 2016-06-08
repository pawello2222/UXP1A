//
// Created by anowikowski on 08.06.16.
//

#include <fcntl.h>
#include <unistd.h>
#include "LindaFileManagerBase.h"
#include "../Exception/FileOperationError.h"

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
ssize_t LindaFileManagerBase<T1, T2>::Read(void *buffer, size_t bytesToRead)
{
    return read(this->m_iFileDescriptor, buffer, bytesToRead);
}

template<typename T1, typename T2>
ssize_t LindaFileManagerBase<T1, T2>::WriteAndSeekBack(char *buffer, size_t size)
{
    ssize_t bytesWritten = write(this->m_iFileDescriptor, buffer, size);
    this->Seek(-bytesWritten, SEEK_CUR);
    return bytesWritten;
}







