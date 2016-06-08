//
// Created by anowikowski on 08.06.16.
//

#ifndef UXP1A_FILEMANAGERBASE_H
#define UXP1A_FILEMANAGERBASE_H


#include <string>

template<typename T>
class LindaFileManagerBase
{
public:
    LindaFileManagerBase(std::string filePath);
    ~LindaFileManagerBase();
    void RemoveEntryTakenFlag();
    ssize_t WriteAndSeekBack(T &obj);
    int LockCurrentEntry();
    int UnlockCurrentEntry();
    void Seek(__off_t offset, int whence);
    ssize_t Read(void *buffer, size_t bytesToRead);
protected:
    static const char FileEntryTakenFlagMask = 0b00000001;
    static const char TupleFileEntryTupleDataEnd = '\x00';
private:
    int m_iFileDescriptor;
    ssize_t WriteAndSeekBack(char *buffer, size_t size);
};

#include "LindaFileManagerBase.tpp"

#endif //UXP1A_FILEMANAGERBASE_H
