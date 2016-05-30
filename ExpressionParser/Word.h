//
// Created by Kacper Harasim on 30.05.2016.
//

#include <string>
#include "Token.h"
#ifndef UXP1A_WORD_H
#define UXP1A_WORD_H

#endif //UXP1A_WORD_H


class Word: public Token {
 private:
  std::string lexeme;

 public:

  Word(const std::string &lexeme, Tag tag) : Token(tag), lexeme(lexeme) { }

  const std::string &getLexeme() const {
    return lexeme;
  }
};