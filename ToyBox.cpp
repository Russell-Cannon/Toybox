#include "Parser.hpp"
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
  Parser parser;

  std::string line;
  while (std::getline(fin, line)) {
    parser.Tokenize(line);
  }
  parser.PrintTokens();
  parser.ParseTokens();
  parser.PrintStatements();

  fin.close();

  return 0;
}
