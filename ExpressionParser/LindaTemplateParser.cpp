//
// Created by Kacper Harasim on 31.05.2016.
//

#include "LindaTemplateParser.h"
#include "Word.h"
#include "Real.h"
#include "Number.h"
#include "Identifier.h"
#include "../Exception/LindaTupleTemplateFormatError.h"


LindaTupleTemplate LindaTemplateParser::parse() {

  Lexer lexer(entry);
  auto firstToken = lexer.scan();
  if (firstToken->tag != Tag::OpenBracket) {
    syntaxException("Template should begin with '('");
  }
  auto itemsWithLeftover = parseItems(lexer);

  if (itemsWithLeftover.second->tag != Tag::EndBracket) {
    syntaxException("Template should end with )");
  }
  auto lastToken = lexer.scan();
  if (lastToken->tag != Tag::END_OF_INPUT) {
    syntaxException(") should be last character of template");
  }

  return LindaTupleTemplate(itemsWithLeftover.first);
}

std::pair<std::vector<LindaTupleItemTemplate>, std::shared_ptr<Token>> LindaTemplateParser::parseItems(Lexer &lexer) {

  std::vector<LindaTupleItemTemplate> recognizedItems;
  while (true) {
    auto first = lexer.scan();
    if (first->tag == Tag::EndBracket) { return std::make_pair(recognizedItems, first); }
    else if (first->tag == Tag::Word) {
      std::shared_ptr<Word> word = std::static_pointer_cast<Word>(first);
      auto itemType = typeBasedOnWord(word);

      auto expectedColon = lexer.scan();
      if (expectedColon->tag != Tag::Colon) {
        syntaxException("Colon should appear after type identifier");
      }

      auto operatorToken = lexer.scan();
      //If star occurs - any of the matches is good ;)
      if (operatorToken->tag == Tag::Star) {
        recognizedItems.push_back(LindaTupleItemTemplate(itemType, LindaTupleItemOperator::all, ""));
      }
      else {
        auto lindaTemplateOperator = operatorBasedOnToken(operatorToken);
        makeSureThatOperatorMatchesType(lindaTemplateOperator, itemType);
        auto value = lexer.scan();
        //Make sure that
        makeSureThatTagMatchesType(value->tag, itemType);
        recognizedItems.push_back(LindaTupleItemTemplate(itemType, lindaTemplateOperator,
                                                         convertedStringValueForToken(value)));
      }
    }
    else {
      syntaxException("After comma or ( character - one of the type specifiers should always occur");
    }
    auto next = lexer.scan();
    if (next->tag == Tag::EndBracket) {
      return std::make_pair(recognizedItems, next);
    }
    else if (next->tag == Tag::Comma) { continue; }
  }
}

LindaTupleItemType LindaTemplateParser::typeBasedOnWord(std::shared_ptr<Word> word) {
  std::string lexeme = word->getLexeme();
  if (lexeme == "integer") {
    return LindaTupleItemType::Integer;
  }
  else if (lexeme == "float") {
    return LindaTupleItemType::Float;
  }
  else if (lexeme == "string") {
    return LindaTupleItemType::String;
  }
  syntaxException("Type should one of the: integer, float, string");
}

void LindaTemplateParser::syntaxException(std::string message) {
  throw LindaTupleTemplateFormatError(message, -1);
}

LindaTupleItemOperator LindaTemplateParser::operatorBasedOnToken(std::shared_ptr<Token> token) {
  switch (token->tag) {
    case Tag::Equals:
      return LindaTupleItemOperator::eq;
    case Tag::GreaterThan:
      return LindaTupleItemOperator::gt;
    case Tag::GreaterThanOrEqual:
      return LindaTupleItemOperator::ge;
    case Tag::LessThan:
      return LindaTupleItemOperator::lt;
    case Tag::LessThanOrEqual:
      return LindaTupleItemOperator::le;
    default: break;
  }
  syntaxException("Only >, <, ==, <=, >= operators should occur");
}

void LindaTemplateParser::makeSureThatTagMatchesType(Tag tag, LindaTupleItemType type) {
  if (type == LindaTupleItemType::Integer && tag != Tag::Integer) {
    syntaxException("When integer identifier is specified, integer variable should occur");
  }
  if (type == LindaTupleItemType::Float && tag != Tag::Float) {
    syntaxException("When float identifier is specified, float variable should occur");
  }
  if (type == LindaTupleItemType::String && tag != Tag::String) {
    syntaxException("When string identifier is specified, string variable should occur");
  }
}

void LindaTemplateParser::makeSureThatOperatorMatchesType(LindaTupleItemOperator lindaTupleItemOperator,
                                                          LindaTupleItemType type) {
  if (type == LindaTupleItemType::Float && (lindaTupleItemOperator == LindaTupleItemOperator::eq
      || lindaTupleItemOperator == LindaTupleItemOperator::ge || lindaTupleItemOperator == LindaTupleItemOperator::le)) {
    syntaxException("==, >=, <= operators for floats is not supported");
  }
}

std::string LindaTemplateParser::convertedStringValueForToken(std::shared_ptr<Token> token) {
  if (token->tag == Tag::Float) {
    std::shared_ptr<Real> real = std::static_pointer_cast<Real>(token);
    return std::to_string(real->getValue());
  }
  else if (token->tag == Tag::Integer) {
    std::shared_ptr<Number> number = std::static_pointer_cast<Number>(token);
    return std::to_string(number->getValue());
  }
  else if (token->tag == Tag::String) {
    std::shared_ptr<Identifier> identifier = std::static_pointer_cast<Identifier>(token);
    return identifier->getValue();
  }
  else {
    //TODO: Should probably throw exception
    return "";
  }
}
