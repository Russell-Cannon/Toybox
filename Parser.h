#pragma once

#include "Lexer.h"
#include "Toy.h"
#include "Types.h"
#include <string>

class Parser {
    Document document;
    std::string filename;
    int lineCount = 1;
    std::string TexturePath = "./checker-map.png";

    bool syntaxError(std::string message);

    template <typename ToyType>
    bool parseNAryOperation(std::string& str, Toy* toy, int arguments);
    template <typename ToyType>
    bool parseUnaryOperation(std::string& str, Toy* toy);
    template <typename ToyType>
    bool parseBinaryOperation(std::string& str, Toy* toy);
    template <typename ToyType>
    bool parseTrinaryOperation(std::string& str, Toy* toy);
    template <typename ToyType>
    bool parseExpandingSizeOperation(std::string& str, Toy* toy);
    bool parseToy(std::string& str, Toy* toy);
    bool parseStatement(std::string& str, Toy* toy);
    bool parseMathematicalOperator(std::string& str, Toy* toy, Toy* first);

    void printToy(Toy* toy, bool last, std::string pre);

  public:
    Parser();
    Parser(std::string _filename);

    void Parse(std::string str);
    std::string GenerateGLSL();
    std::string GetTextureFilePath() const;

    void PrintAST();
};