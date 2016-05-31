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
#include "../Model/LindaWaitingQueueFileEntry.h"
class LindaTupleParser {
 private:
  std::string entry;
  std::pair<std::vector<LindaTupleItem>, std::shared_ptr<Token>> parseItems(Lexer& lexer);
  void syntaxException(std::string message);
 public:

  LindaTupleParser(const LindaTuplesFileEntry &entry) : entry(entry.TupleData) { }
  LindaTupleParser(const LindaWaitingQueueFileEntry &entry): entry(entry.TupleData) {}
  LindaTuple parse();
};


#endif //UXP1A_PARSER_H
