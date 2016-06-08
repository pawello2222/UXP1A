//
// Created by anowikowski on 08.06.16.
//

#include <zconf.h>
#include <cstring>
#include "LindaTuplesFileManager.h"
#include "../Exception/LindaTupleMaxSizeExceeded.h"
#include "../ExpressionParser/LindaTupleParser.h"
#include "../Exception/NoLindaTupleMatchingTemplateException.h"
#include "SemaphoreManager.h"
#include "../Exception/FileOperationError.h"

LindaTuplesFileManager::LindaTuplesFileManager(std::string filePath) : LindaFileManagerBase(filePath)
{

}

LindaTuplesFileEntry LindaTuplesFileManager::CreateFileEntry(LindaTuple &tuple)
{
    LindaTuplesFileEntry entry;
    entry.Flags = FileEntryTakenFlagMask;

    std::string tupleString = tuple.ToString();
    if (tupleString.size() > sizeof(entry.TupleData) - 1)
    {
        throw LindaTupleMaxSizeExceeded();
    }

    strcpy(entry.TupleData, tupleString.c_str());
    entry.TupleData[tupleString.size()] = this->TupleFileEntryTupleDataEnd;

    return entry;
}

LindaTuple LindaTuplesFileManager::ReadAndLock(LindaTupleTemplate &tupleTemplate)
{
    if (!this->FindAndLockFileEntry([this, &tupleTemplate](const LindaTuplesFileEntry &fileEntry) {
        if ((fileEntry.Flags & this->FileEntryTakenFlagMask) != this->FileEntryTakenFlagMask) {
            return false;
        }
        LindaTupleParser parser(fileEntry);
        LindaTuple tuple = parser.parse();
        return tupleTemplate.IsMatch(tuple);
    }, true)) {
        this->UnlockCurrentEntry();
        throw NoLindaTupleMatchingTemplateException();
    }

    LindaTuplesFileEntry fileEntry;
    ssize_t result = this->ReadAndSeekBack(&fileEntry, sizeof(fileEntry));
    if (result == -1 || result != sizeof(fileEntry))
    {
        this->UnlockCurrentEntry();
        throw FileOperationError("Error reading entry from file.", errno);
    }

    LindaTupleParser parser(fileEntry);
    return parser.parse();
}






