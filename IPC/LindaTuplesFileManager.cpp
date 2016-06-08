//
// Created by anowikowski on 08.06.16.
//

#include <zconf.h>
#include <cstring>
#include "LindaTuplesFileManager.h"
#include "../Exception/LockingError.h"
#include "../Exception/LindaFileCorrupt.h"
#include "../Exception/LindaTupleMaxSizeExceeded.h"
#include "../ExpressionParser/LindaTupleParser.h"
#include "../Exception/NoLindaTupleMatchingTemplateException.h"

LindaTuplesFileManager::LindaTuplesFileManager(std::string filePath) : LindaFileManagerBase(filePath)
{

}

void LindaTuplesFileManager::FindAndLockUnusedTupleEntry()
{
    //Seek to file begin
    this->Seek(0, SEEK_SET);

    LindaTuplesFileEntry fileEntry;
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
        this->Seek(sizeof(LindaTuplesFileEntry),  SEEK_CUR);
    } while(true);
}

LindaTuplesFileEntry LindaTuplesFileManager::CreateTupleFileEntry(LindaTuple &tuple)
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
    //Seek to file begin
    this->Seek(0, SEEK_SET);

    LindaTuplesFileEntry fileEntry;
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
            LindaTupleParser parser(fileEntry);
            LindaTuple tuple = parser.parse();
            if (tupleTemplate.IsMatch(tuple))
            {
                return tuple;
            }
        }

        //Remove lock
        this->UnlockCurrentEntry();

        //Seek forward to beginning of next tuple
        this->Seek(sizeof(LindaTuplesFileEntry), SEEK_CUR);
    } while (true);

    throw NoLindaTupleMatchingTemplateException();
}






