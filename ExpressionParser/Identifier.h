//
// Created by Kacper Harasim on 30.05.2016.
//

#include <string>
#include "Token.h"
#ifndef UXP1A_STRING_H
#define UXP1A_STRING_H

#endif //UXP1A_STRING_H

class Identifier: public Token {
 private:
  std::string value;

 public:


  Identifier(const std::string &value) : Token(Tag::String), value(value) { }
  const std::string &getValue() const {
    return value;
  }
};