#pragma once

#include "Types.hpp"
#include <iostream>
#include <regex>

// All tokens with their names and respective patterns
const TokenType TYPE{"Type", R"(^int\b)"},
    IDENTIFIER{"Identifier", R"(^[a-z]\b)"},
    LINE_DELIMITER{"Line Delimiter", R"(^;)"},
    KEY_WORD{"Key Word", R"(^[A-Za-z]+)"}, EQUALS{"Equals", R"(^=(?!=))"},
    EXPRESSION{"Expression", R"(^\b\d+\b)"}, WHILE{"While", R"(^while\b)"},
    IF{"If", R"(^if\b)"}, OPEN_PARENTHESIS{"Open Parenthesis", "^("},
    CLOSE_PARENTHESIS{"Close Parenthesis", "^)"},
    OPEN_CURLY{"Open Curly Brace", "^{"},
    CLOSE_CURLY{"Close Curly Brace", "^}"};
// List of all known tokens
const TokenType TokenTypes[] = {
    TYPE,
    IDENTIFIER,
    LINE_DELIMITER,
    KEY_WORD,
    EQUALS,
    EXPRESSION,
    WHILE,
    IF,
    OPEN_PARENTHESIS,
    CLOSE_PARENTHESIS,
    OPEN_CURLY,
    CLOSE_CURLY,
};

class Lexer {
protected:
  std::deque<Token> tokens;

public:
  static void PrintToken(const Token &t) {
    std::cout << t.name << ": " << t.value << '\n';
  }
  static void PrintSymbol(const SymbolType &t) { std::cout << t.name << '\n'; }

  void Tokenize(std::string str) {
    DEBUG("Tokenizing: ", str);

    std::smatch match;
    while (str.length() > 0) {
      while (str.length() > 0 && str[0] == ' ')
        str = str.substr(1);
      DEBUG("Current string: ", str);
      for (TokenType tt : TokenTypes) {
        if (std::regex_search(str, match, std::regex(tt.pattern))) {
          tokens.push_back({tt, match[0].str()});
          str = str.substr(match.position() + match.length());
          DEBUG("Found: ", tt.name);
          break;
        }
      }
    }
  }

  void PrintTokens() {
    std::cout << "Symbols: {\n";
    for (Token s : tokens) {
      PrintToken(s);
    }
    std::cout << "}\n";
  }
};
