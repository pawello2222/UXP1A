//
// Created by Pawel Wiszenko on 28.04.2016.
//

#include <iostream>
#include "Model/LindaTupleItem.h"
#include "Model/LindaTuple.h"
#include "IPC/LindaTuplePool.h"
#include "ExpressionParser/LindaTupleParser.h"

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
        std::cin >> command;
        if (command == "connect")
        {
            std::string tuplesFilePath, waitingQueueFilePath;
            std::cout << "Tuples file path: ";
            std::cin >> tuplesFilePath;
            std::cout << "Waiting queue file path: ";
            std::cin >> waitingQueueFilePath;
            pool.ConnectPool(tuplesFilePath, waitingQueueFilePath);
        }
        else if (command == "disconnect")
        {
            pool.DisconnectPool();
        }
        else if (command == "input")
        {
            //TODO
        }
        else if (command == "output")
        {

            std::string tupleString;
            std::cout << "Tuple: ";
            std::cin >> tupleString;
            LindaTupleParser parser(tupleString);
            LindaTuple tuple = parser.parse();
            pool.Output(tuple);
        }
        else if (command == "read")
        {
            //TODO
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
