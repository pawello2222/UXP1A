//
// Created by Pawel Wiszenko on 28.04.2016.
//

#include <iostream>
#include "Model/LindaTupleItem.h"
#include "Model/LindaTuple.h"
#include "IPC/LindaTuplePool.h"
#include "ExpressionParser/LindaTupleParser.h"
#include "ExpressionParser/LindaTemplateParser.h"
#include "IPC/SemaphoreManager.h"
#include "Exception/LindaPoolOperationTimedOutException.h"

void display_help()
{
    std::cout << "connect: connects to the pool" << std::endl;
    std::cout << "disconnect: disconnects from the pool" << std::endl;
    std::cout << "input: read tuple and remove it from pool" << std::endl;
    std::cout << "read: read tuple without removing it from pool" << std::endl;
    std::cout << "output: add tuple to pool" << std::endl;
    std::cout << "exit: exit program" << std::endl;
}


int main()
{
    LindaTuplePool pool;
    display_help();

    while(true)
    {
        std::string command;
        std::cout << "Enter command: ";
        std::cin >> command;
        if (command == "connect")
        {
            std::string tuplesFilePath, waitingQueueFilePath;
            std::cout << "Tuples file path: ";
            std::getline(std::cin, tuplesFilePath);
            std::cout << "Waiting queue file path: ";
            std::getline(std::cin, waitingQueueFilePath);
            pool.ConnectPool(tuplesFilePath, waitingQueueFilePath);
        }
        else if (command == "disconnect")
        {
            pool.DisconnectPool();
        }
        else if (command == "input")
        {
            std::string tupleTemplateString;
            unsigned long timeout;
            std::cout << "Tuple template: ";
            std::getline(std::cin, tupleTemplateString);
            std::cout << "Timeout (ms): ";
            std::cin >> timeout;
            LindaTemplateParser parser(tupleTemplateString);
            LindaTupleTemplate tupleTemplate = parser.parse();

            try {
                LindaTuple tuple = pool.Input(tupleTemplate, timeout);
                std::cout << tuple.ToString() << std::endl;
            }
            catch (LindaPoolOperationTimedOutException ex) {
                std::cout << "Timed out." << std::endl;
            }

        }
        else if (command == "output")
        {

            std::string tupleString;
            std::cout << "Tuple: ";
            std::getline(std::cin, tupleString);
            LindaTupleParser parser(tupleString);
            LindaTuple tuple = parser.parse();
            pool.Output(tuple);
        }
        else if (command == "read")
        {
            std::string tupleTemplateString;
            unsigned long timeout;
            std::cout << "Tuple template: ";
            std::getline(std::cin, tupleTemplateString);
            std::cout << "Timeout (ms): ";
            std::cin >> timeout;
            LindaTemplateParser parser(tupleTemplateString);
            LindaTupleTemplate tupleTemplate = parser.parse();

            try {
                LindaTuple tuple = pool.Read(tupleTemplate, timeout);
                std::cout << tuple.ToString() << std::endl;
            }
            catch (LindaPoolOperationTimedOutException ex) {
                std::cout << "Timed out." << std::endl;
            }

        }
        else if (command == "exit")
        {
            break;
        }
        else
        {
            display_help();
        }
    }

    return 0;
}
