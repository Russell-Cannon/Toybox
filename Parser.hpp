#pragma once

#include <deque>
#include <iostream>
#include <regex>
#include <variant>
#include <vector>

#define DEBUG(MSG, VALUE) std::cout << (MSG) << (VALUE) << std::endl

// patterns
struct SymbolType;
struct TokenType;
struct StatementType;
// actual values
struct Token;
struct Statement;
// a symbol can be a token (terminal) or a symbol (token or statement)
typedef std::vector<SymbolType> Sequence;

struct SymbolType { //Everything is a symbol
	std::string name;
};

struct TokenType : SymbolType { //A token is a symbol which can be resolved using a (regex) pattern
	std::string pattern;
};
struct StatementType : SymbolType { //A Statement is a symbol which requires a pattern of symbols
	// List of (List of symbols (pattern)) (possible patterns)
	std::vector<Sequence> Sequences;
};

struct Token : TokenType {
	std::string value;
};
struct Statement : StatementType{
	std::deque<SymbolType> pattern;
};

// All tokens with their names and respective patterns
const TokenType 
	TYPE{"Type", R"(^int\b)"},
	IDENTIFIER{"Identifier", R"(^[a-z]\b)"},
	LINE_DELIMITER{"Line Delimiter", R"(^;)"},
	KEY_WORD{"Key Word", R"(^[A-Za-z]+)"}, 
	EQUALS{"Equals", R"(^=(?!=))"},
	EXPRESSION{"Expression", R"(^\b\d+\b)"}, 
	WHILE{"While", R"(^while\b)"},
	IF{"If", R"(^if\b)"}, 
	OPEN_PARENTHESIS{"Open Parenthesis", "^("},
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

// All possible statements with their names and possible patterns
const StatementType 
	ASSIGNMENT{
		"Assignment",
		{
			{IDENTIFIER, EQUALS, EXPRESSION, LINE_DELIMITER}
		}
	},
	DECLARATION{
		"Declaration",
		{
			{TYPE, IDENTIFIER, LINE_DELIMITER},
			{TYPE, ASSIGNMENT}
		}
	};

// List of all possible statements
const StatementType StatementTypes[] {DECLARATION, ASSIGNMENT};

class Parser {
public:
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

	void ParseTokens() {
		std::deque<Token> tempTokens;
		for (int i = tokens.size() - 1; i >= 0; i--)
			tempTokens.push_front(tokens[i]);

		int maxSize = tempTokens.size();
		for (int i = 0; i < maxSize && tempTokens.size() > 0; i++) {
			for (StatementType statement : StatementTypes) { // Check against every type of statement
				for (int j = 0; j < statement.Sequences.size(); j++) { //
					if (match(tempTokens, statement.Sequences[i])) // compare current list of tokens to desired list in statement
						addStatement(tempTokens, statement, statement.Sequences[i]);
				}
			}
		}
	}

	void PrintTokens() {
		std::cout << "Symbols: {\n";
		for (Token s : tokens) {
			printToken(s);
		}
		std::cout << "}\n";
	}

  void PrintStatements() {
    std::cout << "Statements: {\n";
    for (Statement s : statements) {
      printStatement(s);
    }
    std::cout << "}\n";
  }

protected:
  void printToken(const Token &t) {
    std::cout << t.type.name << ": " << t.value << '\n';
  }
  void printSymbol(const SymbolType &t) {
    std::cout << t.name << '\n';
  }
  void printStatement(const Statement &s) {
    std::cout << s.type.name << ":\n";
    for (SymbolType s : s.pattern) {
      std::cout << '\t';
      printSymbol(s);
    }
  }
  bool match(const std::deque<Token> &list,
             const std::vector<SymbolType> &expression) {
    if (expression.size() > list.size()) // not enough room to fit.
      return false;

    for (int i = 0; i < expression.size(); i++) {
      // if (expression[i] != list[i].type)
      if (expression[i].name != list[i].type.name)
        return false;
    }

    return true;
  }
  void addStatement(std::deque<Token> &list, const StatementType &type,
                    const Sequence &pattern) {
    Statement toAdd;
    toAdd.type = type;

    std::deque<Token> tokensInStatement;
    for (int i = 0; i < pattern.size(); i++) {
      Token t = list[0];
      list.pop_front();
      tokensInStatement.push_back(t);
    }

    toAdd.pattern = tokensInStatement;
    statements.push_back(toAdd);
  }

  std::deque<Symbol> tokens;
  std::deque<Statement> statements;
};
