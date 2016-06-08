//
// Created by Kacper Harasim on 08.05.2016.
//

#include <iostream>
#include "Lexer.h"
#include "Number.h"
#include "Real.h"
#include "Identifier.h"
#include "Word.h"
#include "../Exception/UnrecognizedCharacter.h"
#include "../Exception/UnrecognizedToken.h"

char Lexer::readCharFromInput() {
  char c;
  if (inputIterator == input.size()) {
    c = '\0';
  } else {
    c =  input[inputIterator];
    inputIterator++;
  }
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
  inputIterator = 0;
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
    return std::make_shared<Token>(Tag::OpenBracket);
  }
  else if (peek == ')') {
    return std::make_shared<Token>(Tag::EndBracket);
  }
  else if (peek == ',') {
    return std::make_shared<Token>(Tag::Comma);
  }
  else if (peek == ':') {
    return std::make_shared<Token>(Tag::Colon);
  }
  else if (peek == '*') {
    return std::make_shared<Token>(Tag::Star);
  }
  else if (peek == '=') {
    readChar();
    if (peek != '=') { unrecognizedTokenException("= should be followed by another ="); }
    return std::make_shared<Token>(Tag::Equals);
  }
  else if (peek == '<') {
    readChar();
    if (peek == '=') { return std::make_shared<Token>(Tag::LessThanOrEqual); }
    backChar();
    return std::make_shared<Token>(Tag::LessThan);
  }
  else if (peek == '>') {
    readChar();
    if (peek == '=') { return std::make_shared<Token>(Tag::GreaterThanOrEqual); }
    backChar();
    return std::make_shared<Token>(Tag::GreaterThan);
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
    else if (isalpha(peek)) {
    std::string buffer;
    while (true) {
      buffer += peek;
      readChar();
      if (!isalpha(peek)) { backChar(); return std::make_shared<Word>(buffer); }
    }
  }
    if (peek == '\0' || peek == '\n') {
      return std::make_shared<Token>(Tag::END_OF_INPUT);
    }
  else {
      std::string peekInString(1, peek);
      throw UnrecognizedCharacter("Unrecognized token found: " + peekInString);
  }
}
void Lexer::unrecognizedTokenException(std::string msg) {
  throw UnrecognizedToken(msg);
}