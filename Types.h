#pragma once
#include <regex>
#include <string>

#define DEBUG(MSG, VALUE)
// #define DEBUG(MSG, VALUE) std::cout << (MSG) << (VALUE) << std::endl

struct TokenType;
struct Token;

struct TokenType { // A token is a symbol which can be resolved using a (regex) pattern
    std::string name;
    std::regex pattern;
    bool operator==(const TokenType& other) const {
        return name == other.name;
    }
    bool operator!=(const TokenType& other) const {
        return name != other.name;
    }
};
struct Token {
    const TokenType type;
    std::string value;
    std::string ToString() {
        return type.name + ": " + value;
    }
};

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