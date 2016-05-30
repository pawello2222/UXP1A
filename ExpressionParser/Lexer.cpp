//
// Created by Kacper Harasim on 08.05.2016.
//

#include <iostream>
#include "Lexer.h"
#include "Word.h"
#include "Number.h"
#include "Real.h"
#include "Identifier.h"

char Lexer::readCharFromInput() {
  if (inputIterator == input.end()) {
    //TODO: Throw exception
  }
  char c =  *inputIterator;
  inputIterator++;
  return c;
}

void Lexer::readChar() {
  std::cout << "Reading char\n";
  peek = readCharFromInput();
}

void Lexer::backChar() {
  --inputIterator;
}

Lexer::Lexer(std::string input): input(input)  {
  inputIterator = input.begin();
}

std::shared_ptr<Token> Lexer::scan() {
  while (true) {
    readChar();
    if (peek == ' ' || peek == '\t') {
      continue;
    }
    else { break; }
  }

  if (peek == '(') {
    return std::make_shared<Word>("(", Tag::OpenBracket);
  }
  else if (peek == ')') {
    return std::make_shared<Word>(")", Tag::EndBracket);
  }
  else if (peek == ',') {
    return std::make_shared<Word>(",", Tag::Comma);
  }
  else if (isdigit(peek)) {
    int value = 0;
    do {
      value = 10 * value + peek - '0';
      readChar();
    } while (isdigit(peek));
    if (peek != '.') {
      backChar();
      return std::make_shared<Number>(value);
    }
    double doubleValue = value;
    double x = 10;
    while (true) {
      readChar();
      if (!isdigit(peek)) {
        backChar();
        break;
      }
      doubleValue = doubleValue + (peek - '0') / x;
      x *= 10;
    }
    return std::make_shared<Real>(doubleValue);
  }
    else if (peek == '\"') {
    std::string buffer;
    while (true) {
        readChar();
        if (peek == '\"') { return std::make_shared<Identifier>(buffer); }
        buffer += peek;
      }
    }
    if (peek == '\0') {
      return std::make_shared<Token>(Tag::END_OF_INPUT);
    }
  else {
    //TODO:: Throw exception
  }
}