//
// Created by Kacper Harasim on 08.05.2016.
//

#ifndef UXP1A_LEXER_H
#define UXP1A_LEXER_H


#include <string>
#include "Token.h"
class Lexer {

 private:
  char peek = ' ';
  std::string::iterator inputIterator;
  std::string input;
  char readCharFromInput();
  void readChar();
  void backChar();

 public:
  Lexer(std::string input);
  std::shared_ptr<Token> scan();

};


#endif //UXP1A_LEXER_H
