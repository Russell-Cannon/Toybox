#pragma once
#include <vector>
#include <string>
#include <deque>

// #define DEBUG(MSG, VALUE) std::cout << (MSG) << (VALUE) << std::endl
#define DEBUG(MSG, VALUE)

// patterns
struct SymbolType;
struct TokenType;
struct StatementType;
// actual values
struct Token;
struct Statement;
// a symbol can be a token (terminal) or a symbol (token or statement)
typedef std::vector<SymbolType> SymbolSequence;

struct SymbolType { //Everything is a symbol
	std::string name;
};

struct TokenType : SymbolType { //A token is a symbol which can be resolved using a (regex) pattern
	std::string pattern;
};
struct StatementType : SymbolType { //A Statement is a symbol which requires a pattern of symbols
	// List of (List of symbols (pattern)) (possible patterns)
	std::vector<SymbolSequence> Sequences;
};

struct Token : TokenType {
	std::string value;
};
struct Statement : StatementType{
	std::deque<SymbolType> Sequence;
};
