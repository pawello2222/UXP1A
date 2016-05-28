//
// Created by anowikowski on 27.05.16.
//

#include "LindaTuplePool.h"
#include "../Exception/LindaTuplePoolConnectionError.h"
#include "../Exception/InvalidOperation.h"
#include <fcntl.h>
#include <unistd.h>

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
    this->m_iTuplesFd = open(lindaTuplesFilePath.c_str(), O_RDWR | O_CREAT);
    if (this->m_iTuplesFd == -1)
    {
        throw LindaTuplePoolConnectionError("Error when opening tuples file.", errno);
    }

    this->m_iWaitingQueueFd = open(lindaWaitingQueueFilePath.c_str(), O_RDWR | O_CREAT);
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
    //TODO
}

void LindaTuplePool::GuardPoolConnected()
{
    if (!this->m_bIsConnected)
    {
        throw InvalidOperation();
    }
}
