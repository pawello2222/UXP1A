//
// Created by anowikowski on 28.05.16.
//

#ifndef UXP1A_LINDAFILECORRUPT_H
#define UXP1A_LINDAFILECORRUPT_H


#include <exception>
#include "MessageExceptionBase.h"

class LindaFileCorrupt : public MessageExceptionBase
{
public:
    LindaFileCorrupt();
};


#endif //UXP1A_LINDAFILECORRUPT_H
