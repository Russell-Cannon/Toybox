#pragma once

#include "Types.hpp"
#include <cctype>
#include <cmath>
#include <iostream>
#include <regex>

// All tokens with their names and respective patterns
const TokenType
    IDENTIFIER{"Identifier", std::regex("^[a-z]+", std::regex::icase)},
    FILEPATH{"File Path", std::regex("^(\\.\\.?\\/)+[a-z ._-]*\\.[a-z0-9]+", std::regex::icase)},
    OPEN_PARENTHESIS{"Open Parenthesis", std::regex("^\\(")},
    CLOSE_PARENTHESIS{"Close Parenthesis", std::regex("^\\)")},
    COMMA{"Comma", std::regex("^,")},
    NUMBER{"Number", std::regex("(^-?[0-9]*\\.[0-9]+[fd]?|^-?[0-9]+[fd]?)", std::regex::icase)},
    OPERATOR{"Mathematical Operator", std::regex("^[+-/*]")},
    EQUALS{"Equals Sign", std::regex("^=")},
    AXIS{"Axis", std::regex("^\\.[xyzwrgbastpquv]", std::regex::icase)},
    EOI{"END OF INPUT", std::regex("\\b\\B")},
    LINE_COMMENT{"Line comment", std::regex("^\\/\\/.*(\\n|$)")},
    MULTI_LINE_COMMENT{"Multi or inline comment", std::regex("^\\/\\*(.|\\n)*\\*\\/")},
    ERROR{"UNKNOWN SYMBOL", std::regex("\\b\\B")};

// List of tokens which are checked against
const TokenType TokenTypes[] = {
    IDENTIFIER,
    FILEPATH,
    OPEN_PARENTHESIS,
    CLOSE_PARENTHESIS,
    COMMA,
    NUMBER,
    OPERATOR,
    EQUALS,
    AXIS};

class Lexer {
  public:
    Lexer() {}

    Token GetNext(std::string& str, int& _lineCount) {
        std::smatch match;

        while (str.length() > 0 && (str[0] == ' ' || str[0] == '\n' || str[0] == '\t')) {
            if (str[0] == '\n')
                _lineCount++;
            str = str.substr(1);
        }

        if (str.length() == 0)
            return {EOI, "Empty String"};

        if (std::regex_search(str, match, LINE_COMMENT.pattern)) {
            str = str.substr(match[0].length());
            _lineCount++;
            return GetNext(str, _lineCount); // skip to next token
        }
        if (std::regex_search(str, match, MULTI_LINE_COMMENT.pattern)) {
            for (char c : match[0].str())
                if (c == '\n')
                    _lineCount++;

            str = str.substr(match[0].length());
            return GetNext(str, _lineCount); // skip to next token
        }

        DEBUG("Current string: ", str);
        for (TokenType tt : TokenTypes) {
            if (std::regex_search(str, match, tt.pattern)) {
                std::string found = match[0].str();
                std::transform(found.begin(), found.end(), found.begin(), ::tolower);
                DEBUG("Found: ", tt.name);
                str = str.substr(found.length());
                return {tt, found};
            }
        }
        std::cerr << "UNKNOWN CHARACTER: in '" << str << "'\n";
        return {ERROR, str};
    }

    Token PeekNext(std::string& str) {
        int n = 0;
        std::string temp(str);
        return GetNext(temp, n);
    }
};
