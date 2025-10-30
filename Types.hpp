#pragma once
#include <vector>
#include <string>
#include <deque>
#include <regex>
#include <iostream>

#define DEBUG(MSG, VALUE)
// #define DEBUG(MSG, VALUE) std::cout << (MSG) << (VALUE) << std::endl

struct TokenType;
struct Token;

struct TokenType { //A token is a symbol which can be resolved using a (regex) pattern
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
	void Print() {
		std::cout << type.name << ": " << value << '\n';
	}
};
