//
// Created by Kacper Harasim on 04.06.2016.
//

#include "LindaTupleOperationTimedOutException.h"


LindaTupleOperationTimedOutException::LindaTupleOperationTimedOutException(std::string message, int errorCode)
    : ErrorCodeMessageExceptionBase(message, errorCode) {

}
