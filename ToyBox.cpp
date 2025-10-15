#include "Lexer.hpp"
#include <fstream>
#include <iostream>
#include <regex>
#include <string>

int main(int argc, char *argv[]) {
  std::ifstream fin;

  if (argc > 1) {
    fin.open(argv[1]);
  } else {
    std::cout << "Enter file name: ";
    std::string fileName = "first.tb"; // fix on release
    // std::cin >> fileName;
    fin.open(fileName);
  }
  Lexer tokenizer;

  std::string line;
  while (std::getline(fin, line)) {
    tokenizer.Tokenize(line);
  }
  tokenizer.PrintTokens();

  fin.close();

  return 0;
}
