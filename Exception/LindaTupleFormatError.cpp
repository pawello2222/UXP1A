//
// Created by Kacper Harasim on 05.06.2016.
//

#include "LindaTupleFormatError.h"

LindaTupleFormatError::LindaTupleFormatError(std::string message, int errorCode)
    : ErrorCodeMessageExceptionBase(message, errorCode) {

}
