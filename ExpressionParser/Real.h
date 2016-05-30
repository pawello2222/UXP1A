//
// Created by Kacper Harasim on 30.05.2016.
//

#include "Token.h"
#ifndef UXP1A_REAL_H
#define UXP1A_REAL_H

#endif //UXP1A_REAL_H

class Real: public Token {

 private:
  float value;
 public:


  Real(float value) : Token(Tag::Float), value(value) { }
  float getValue() const {
    return value;
  }
};