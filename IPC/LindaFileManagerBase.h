//
// Created by anowikowski on 08.06.16.
//

#ifndef UXP1A_FILEMANAGERBASE_H
#define UXP1A_FILEMANAGERBASE_H


#include <string>
#include <algorithm>

template<typename T1, typename T2>
class LindaFileManagerBase
{
public:
    LindaFileManagerBase(std::string filePath);
    ~LindaFileManagerBase();
    void RemoveEntryTakenFlag();
    ssize_t WriteAndSeekBack(T2 &obj);
    int LockCurrentEntry();
    int UnlockCurrentEntry();
    void Seek(__off_t offset, int whence);
    ssize_t ReadAndSeekBack(void *buffer, size_t bytesToRead);
    void FindAndLockUnusedFileEntry();
protected:
    ssize_t WriteFileEntryAndSeekBack(T1 &obj);
    static const char FileEntryTakenFlagMask = 0b00000001;
    static const char TupleFileEntryTupleDataEnd = '\x00';
    virtual T1 CreateFileEntry(T2 &obj) = 0;
    bool FindAndLockFileEntry(std::function<bool(T1)> predicate, bool processOnlyFirstMatchingPredicate, std::function<void(T1 foundFileEntry)> onFound = [](const T1 &fileEntry){});
private:
    int m_iFileDescriptor;
    ssize_t WriteAndSeekBack(char *buffer, size_t size);
};

#include "LindaFileManagerBase.tpp"

#endif //UXP1A_FILEMANAGERBASE_H
