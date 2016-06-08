//
// Created by anowikowski on 27.05.16.
//

#include "LindaTuplePool.h"
#include "../Exception/LindaTuplePoolConnectionError.h"
#include "../Exception/InvalidOperation.h"
#include "SemaphoreManager.h"
#include "../Exception/LindaPoolOperationTimedOutException.h"
#include "../Exception/NoLindaTupleMatchingTemplateException.h"
#include "../Utilities/TimeUtilities.h"
#include <cstring>
#include <iostream>



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
    try {
        this->m_pTuplesFileManager = std::unique_ptr<LindaTuplesFileManager>(new LindaTuplesFileManager(lindaTuplesFilePath));
    }
    catch (FileOperationError) {
        throw LindaTuplePoolConnectionError("Error when opening tuples file.", errno);
    }

    try {
        this->m_pQueueFileManager = std::unique_ptr<LindaQueueFileManager>(new LindaQueueFileManager(lindaWaitingQueueFilePath));
    }
    catch (FileOperationError) {
        this->m_pTuplesFileManager = std::unique_ptr<LindaTuplesFileManager>();
        throw LindaTuplePoolConnectionError("Error when opening waiting queue file.", errno);
    }

    this->m_bIsConnected = true;
}

void LindaTuplePool::DisconnectPool()
{
    if (this->m_bIsConnected)
    {
        this->m_pTuplesFileManager = std::unique_ptr<LindaTuplesFileManager>();
        this->m_pQueueFileManager = std::unique_ptr<LindaQueueFileManager>();
        this->m_bIsConnected = false;
    }
}

LindaTuple LindaTuplePool::ReadInputInternal(LindaTupleTemplate &tupleTemplate, unsigned long timeout, bool removeTuple)
{
    this->GuardPoolConnected();
    this->m_pQueueFileManager->AddMeToWaitingQueueForTemplate(tupleTemplate);
    try {
        LindaTuple tuple = this->m_pTuplesFileManager->ReadAndLock(tupleTemplate);
        if (removeTuple) {
            this->m_pTuplesFileManager->RemoveEntryTakenFlag();
        }
        this->m_pTuplesFileManager->UnlockCurrentEntry();
        this->m_pQueueFileManager->RemoveMeFromWaitingQueue();
        return tuple;
    }
    catch (NoLindaTupleMatchingTemplateException ex) {
        auto startTime = TimeUtilities::getCurrentTime();
        do {
            long millisecondsToSubtractFromTimeout = TimeUtilities::calculateDifferenceBetweenTimesInMilliseconds(TimeUtilities::getCurrentTime(), startTime);
            if (millisecondsToSubtractFromTimeout < timeout) {
                unsigned long effectiveTimeout = timeout - millisecondsToSubtractFromTimeout;
                if (SemaphoreManager::LockOnSemaphoreWithTimeout(effectiveTimeout) == -1) {
                    this->m_pQueueFileManager->RemoveMeFromWaitingQueue();
                    throw LindaPoolOperationTimedOutException();
                }
            }

            try {
                LindaTuple tuple = this->m_pTuplesFileManager->ReadAndLock(tupleTemplate);
                if (removeTuple) {
                    this->m_pTuplesFileManager->RemoveEntryTakenFlag();
                }
                this->m_pTuplesFileManager->UnlockCurrentEntry();
                this->m_pQueueFileManager->RemoveMeFromWaitingQueue();
                return tuple;
            }
            catch (NoLindaTupleMatchingTemplateException ex) {
                continue;
            }
        }
        while (true);
    }
    catch (MessageExceptionBase ex) {
        this->m_pTuplesFileManager->UnlockCurrentEntry();
        this->m_pQueueFileManager->RemoveMeFromWaitingQueue();
        throw ex;
    }
}

LindaTuple LindaTuplePool::Read(LindaTupleTemplate& tupleTemplate, unsigned long timeout)
{
    return this->ReadInputInternal(tupleTemplate, timeout, false);
}

LindaTuple LindaTuplePool::Input(LindaTupleTemplate& tupleTemplate, unsigned long timeout)
{
    return this->ReadInputInternal(tupleTemplate, timeout, true);
}

void LindaTuplePool::Output(LindaTuple &tuple)
{
    this->GuardPoolConnected();
    this->m_pTuplesFileManager->FindAndLockUnusedFileEntry();

    this->m_pTuplesFileManager->WriteAndSeekBack(tuple);
    this->m_pQueueFileManager->NotifyProcessesWaitingForTuple(tuple);
    this->m_pTuplesFileManager->UnlockCurrentEntry();
}

void LindaTuplePool::GuardPoolConnected()
{
    if (!this->m_bIsConnected)
    {
        throw InvalidOperation("Not connected to the pool.");
    }
}
