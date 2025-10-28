#pragma once

#include "Types.hpp"
#include <cmath>
#include <iostream>
#include <regex>
#include <cctype>

// All tokens with their names and respective patterns
const TokenType
    IDENTIFIER{"Identifier", std::regex("^[a-z]+", std::regex::icase)},
    OPEN_PARENTHESIS{"Open Parenthesis", std::regex("^\\(")},
    CLOSE_PARENTHESIS{"Close Parenthesis", std::regex("^\\)")},
    COMMA{"Comma", std::regex("^,")},
    PERIOD{"Period", std::regex("^\\.")},
    NUMBER{"Number", std::regex("^[-+]?[0-9\\.]+[fd]?", std::regex::icase)},
    AXES{"Axis", std::regex("^[xyzwrgbastpquv]{1,4}", std::regex::icase)},
    ERROR{"UNKNOWN SYMBOL", std::regex("\\b\\B")};

// List of tokens which are checked against
const TokenType TokenTypes[] = {
    IDENTIFIER,
    OPEN_PARENTHESIS,
    CLOSE_PARENTHESIS,
    COMMA,
    PERIOD,
    NUMBER,
    AXES,
    ERROR,
};

class Lexer {
  std::string file;

public:
  Lexer() { }
  Lexer(std::string str) : file(str) { }

  Token GetNext(std::string& str) {
    std::smatch match;
    if (str.length() == 0) return {ERROR, "Empty String"};

    while (str.length() > 0 && str[0] == ' ')
      str = str.substr(1);

    DEBUG("Current string: ", str);
    for (TokenType tt : TokenTypes) {
      if (std::regex_search(str, match, tt.pattern)) {
        std::string found = match[0].str();        
        str = str.substr(found.length());
        DEBUG("Found: ", tt.name);
        std::transform(found.begin(), found.end(), found.begin(), ::tolower);
        return {tt, found};
      }
    }
    std::cerr << "UNKNOWN CHARACTER: in '" << str << "'\n";
    return {ERROR, str};
  }

  Token GetNext() {
    return GetNext(file);
  }

  bool Done() {
    return file.length() == 0;
  }
};
