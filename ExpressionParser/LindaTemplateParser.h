//
// Created by Kacper Harasim on 31.05.2016.
//

#ifndef UXP1A_LINDATEMPLATEPARSER_H
#define UXP1A_LINDATEMPLATEPARSER_H


#include <string>
#include "../Model/LindaTupleItemTemplate.h"
#include "Token.h"
#include "Lexer.h"
#include "../Model/LindaTuplesFileEntry.h"
#include "../Model/LindaWaitingQueueFileEntry.h"
#include "../Model/LindaTupleTemplate.h"
#include "Word.h"

class LindaTemplateParser {

 private:
  std::string entry;
  std::pair<std::vector<LindaTupleItemTemplate>, std::shared_ptr<Token>> parseItems(Lexer& lexer);
  void syntaxException(std::string message);

  LindaTupleItemType typeBasedOnWord(std::shared_ptr<Word>);
  LindaTupleItemOperator operatorBasedOnToken(std::shared_ptr<Token>);
  void makeSureThatTagMatchesType(Tag tag, LindaTupleItemType type);
  void makeSureThatOperatorMatchesType(LindaTupleItemOperator lindaTupleItemOperator, LindaTupleItemType type);
  std::string convertedStringValueForToken(std::shared_ptr<Token> token);
 public:

  LindaTemplateParser(const LindaTuplesFileEntry &entry) : entry(entry.TupleData) { }
  LindaTemplateParser(const LindaWaitingQueueFileEntry &entry): entry(entry.TupleData) {}
  LindaTemplateParser(const std::string &lindaTemplateString): entry(lindaTemplateString) { }
  LindaTupleTemplate parse();
};


#endif //UXP1A_LINDATEMPLATEPARSER_H
