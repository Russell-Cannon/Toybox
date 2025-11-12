#include "Parser.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>
#include <string>

int main(int argc, char *argv[]) {
  std::ifstream fin;
  std::string filename;

  if (argc > 1) {
    filename = argv[1];
  } else {
    std::cout << "Enter file name: ";
    filename = "first.tb"; // fix on release
    // std::cin >> filename;
  }
  fin.open(filename);
  
  std::stringstream buffer;
  buffer<<fin.rdbuf();
  fin.close();
  Parser parser(filename);

  parser.Parse(buffer.str());
  parser.GenerateGLSL("out.html");

  return 0;
}
