//
// Created by anowikowski on 08.06.16.
//

#ifndef UXP1A_LINDATUPLESFILEMANAGER_H
#define UXP1A_LINDATUPLESFILEMANAGER_H


#include <string>
#include "LindaFileManagerBase.h"
#include "../Model/LindaTuplesFileEntry.h"
#include "../Model/LindaTuple.h"
#include "../Model/LindaTupleTemplate.h"

class LindaTuplesFileManager : public LindaFileManagerBase<LindaTuplesFileEntry, LindaTuple>
{
public:
    LindaTuplesFileManager(std::string filePath);
    LindaTuplesFileEntry CreateFileEntry(LindaTuple &tuple);
    LindaTuple ReadAndLock(LindaTupleTemplate &tupleTemplate);
};


#endif //UXP1A_LINDATUPLESFILEMANAGER_H
