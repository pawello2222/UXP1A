//
// Created by Kacper Harasim on 04.06.2016.
//

#ifndef UXP1A_LINDATUPLEOPERATIONTIMEDOUTEXCEPTION_H
#define UXP1A_LINDATUPLEOPERATIONTIMEDOUTEXCEPTION_H


#include "ErrorCodeMessageExceptionBase.h"
class LindaTupleOperationTimedOutException: public ErrorCodeMessageExceptionBase
{
 public:
  LindaTupleOperationTimedOutException(std::string message, int errorCode);
};


#endif //UXP1A_LINDATUPLEOPERATIONTIMEDOUTEXCEPTION_H
