#include "Parser.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>
#include <string>

int main(int argc, char *argv[]) {
  std::ifstream fin;

  if (argc > 1) {
    fin.open(argv[1]);
  } else {
    std::cout << "Enter file name: ";
    std::string fileName = "first.tb"; // fix on release
    std::cin >> fileName;
    fin.open(fileName);
  }
  
  std::stringstream buffer;
  buffer<<fin.rdbuf();
  fin.close();
  Parser parser;

  parser.Parse(buffer.str());
  parser.GenerateGLSL("out.html");

  return 0;
}
