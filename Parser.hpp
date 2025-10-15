#pragma once

#include "Lexer.hpp"
#include "Types.hpp"
#include <iostream>

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
protected:
  std::deque<Statement> statements;

  bool match(const std::deque<Token> &list,
             const std::vector<SymbolType> &expression) {
    if (expression.size() > list.size()) // not enough room to fit.
      return false;

    for (int i = 0; i < expression.size(); i++) {
      // if (expression[i] != list[i].type)
      if (expression[i].name != list[i].name)
        return false;
    }

    return true;
  }
  void addStatement(std::deque<Token> &list, const StatementType &type,
                    const SymbolSequence &pattern) {
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

public:
  void ParseTokens() {
    std::deque<Token> tempTokens;
    for (int i = tokens.size() - 1; i >= 0; i--)
      tempTokens.push_front(tokens[i]);

    int maxSize = tempTokens.size();
    for (int i = 0; i < maxSize && tempTokens.size() > 0; i++) {
      for (StatementType statement :
           StatementTypes) { // Check against every type of statement
        for (int j = 0; j < statement.Sequences.size(); j++) { //
          if (match(tempTokens,
                    statement.Sequences[i])) // compare current list of tokens
                                             // to desired list in statement
            addStatement(tempTokens, statement, statement.Sequences[i]);
        }
      }
    }
  }
  void PrintStatements() {
    std::cout << "Statements: {\n";
    for (Statement s : statements) {
      std::cout << s.name << '\n';
    }
    std::cout << "}\n";
  }
};