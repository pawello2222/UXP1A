//
// Created by Kacper Harasim on 08.05.2016.
//

#ifndef UXP1A_PARSER_H
#define UXP1A_PARSER_H


#include <vector>
#include "../Model/LindaTuplesFileEntry.h"
#include "../Model/LindaTupleItem.h"
#include "Token.h"
#include "../Model/LindaTuple.h"
#include "Lexer.h"
class Parser {
 private:
  LindaTuplesFileEntry entry;
  std::pair<std::vector<LindaTupleItem>, std::shared_ptr<Token>> parseItems(Lexer& lexer);
  void syntaxException(std::string message);
 public:

  Parser(const LindaTuplesFileEntry &entry) : entry(entry) { }
  LindaTuple parse();
};


#endif //UXP1A_PARSER_H
