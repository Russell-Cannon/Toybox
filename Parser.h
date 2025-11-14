#pragma once

#include "Lexer.h"
#include "Toy.h"
#include "Types.h"
#include <string>

class Parser {
    Document document;
    std::string filename;
    int lineCount = 1;

  public:
    std::string TexturePath = "./checker-map.png";
    Parser();
    Parser(std::string _filename);

    bool SyntaxError(std::string message);

    void Parse(std::string str);
    std::string GenerateGLSL();
    template <typename ToyType>
    bool ParseNAryOperation(std::string& str, Toy* toy, int arguments);
    template <typename ToyType>
    bool ParseUnaryOperation(std::string& str, Toy* toy);
    template <typename ToyType>
    bool ParseBinaryOperation(std::string& str, Toy* toy);
    template <typename ToyType>
    bool ParseTrinaryOperation(std::string& str, Toy* toy);
    template <typename ToyType>
    bool ParseExpandingSizeOperation(std::string& str, Toy* toy);
    bool ParseToy(std::string& str, Toy* toy);
    bool ParseStatement(std::string& str, Toy* toy);
    bool ParseMathematicalOperator(std::string& str, Toy* toy, Toy* first);

    void PrintAST();
    void PrintToy(Toy* toy, bool last, std::string pre);
};