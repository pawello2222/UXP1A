//
// Created by Kacper Harasim on 30.05.2016.
//

#include "Token.h"
#ifndef UXP1A_NUMBER_H
#define UXP1A_NUMBER_H

#endif //UXP1A_NUMBER_H

class Number: public Token {

 private:
  int value;


 public:

  Number( int value) : Token(Tag::Integer), value(value) { }

  int getValue() const {
    return value;
  }
};