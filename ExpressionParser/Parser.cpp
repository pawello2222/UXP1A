//
// Created by Kacper Harasim on 08.05.2016.
//

#include "Parser.h"
#include "Lexer.h"
#include "Real.h"
#include "Number.h"
#include "Identifier.h"

void Parser::syntaxException(std::string message) {

}

LindaTuple Parser::parse() {
  Lexer lexer(entry.TupleData);
  auto firstToken = lexer.scan();
  if (firstToken->tag != Tag::OpenBracket) {
    syntaxException("Tuple should begin with '('");
  }
  auto itemsWithLeftover = parseItems(lexer);

  if (itemsWithLeftover.second->tag != Tag::EndBracket) {
    syntaxException("Item should end with )");
  }
  auto lastToken = lexer.scan();
  if (lastToken->tag != Tag::END_OF_INPUT) {
    syntaxException(") should be last character");
  }

  return LindaTuple(itemsWithLeftover.first);
}

std::pair<std::vector<LindaTupleItem>, std::shared_ptr<Token>> Parser::parseItems(Lexer& lexer) {

  std::vector<LindaTupleItem> recognizedItems;
  while (true) {
    auto first = lexer.scan();
    if (first->tag == Tag::EndBracket) { return std::make_pair(recognizedItems, first); }
    else if (first->tag == Tag::Float) {
      std::shared_ptr<Real> real = std::static_pointer_cast<Real>(first);
      recognizedItems.push_back(LindaTupleItem(real->getValue()));
    }
    else if (first->tag == Tag::Integer) {
      std::shared_ptr<Number> number = std::static_pointer_cast<Number>(first);
      recognizedItems.push_back(LindaTupleItem(number->getValue()));
    }
    else if (first->tag == Tag::String) {
      std::shared_ptr<Identifier> identifier = std::static_pointer_cast<Identifier>(first);
      recognizedItems.push_back(LindaTupleItem(identifier->getValue()));
    }
    else {
      syntaxException("After ( - int, float or string should occur");
    }

    auto next = lexer.scan();
    if (next->tag == Tag::EndBracket) {
      return std::make_pair(recognizedItems, first);
    }
    else if (next->tag == Tag::Comma) { continue; }
    else { syntaxException("Item should be followed by either comma or end bracket"); }
  }
}

