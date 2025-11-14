#pragma once

#include "Types.h"

class Lexer {
  public:
    Lexer();
    Token GetNext(std::string& str, int& _lineCount);
    Token PeekNext(std::string& str);
};
