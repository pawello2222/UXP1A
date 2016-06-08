//
// Created by Kacper Harasim on 30.05.2016.
//

#ifndef UXP1A_LINDAWAITINGQUEUEFILEENTRY_H
#define UXP1A_LINDAWAITINGQUEUEFILEENTRY_H


struct LindaWaitingQueueFileEntry
{
    char Flags;
    char TupleTemplateData[251];
    int processId;
};
#endif //UXP1A_LINDAWAITINGQUEUEFILEENTRY_H
