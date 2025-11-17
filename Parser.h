#pragma once

#include "Lexer.h"
#include "Toy.h"
#include "Types.h"
#include <memory>
#include <string>

class Parser {
    std::shared_ptr<Toy> document;
    std::string filename;
    int lineCount = 1;
    std::string TexturePath = "./checker-map.png";

    bool syntaxError(std::string message);

    template <typename ToyType>
    bool parseNAryOperation(std::string& str, std::shared_ptr<Toy> toy, int arguments);
    template <typename ToyType>
    bool parseUnaryOperation(std::string& str, std::shared_ptr<Toy> toy);
    template <typename ToyType>
    bool parseBinaryOperation(std::string& str, std::shared_ptr<Toy> toy);
    template <typename ToyType>
    bool parseTrinaryOperation(std::string& str, std::shared_ptr<Toy> toy);
    template <typename ToyType>
    bool parseExpandingSizeOperation(std::string& str, std::shared_ptr<Toy> toy);
    bool parseToy(std::string& str, std::shared_ptr<Toy> toy);
    bool parseStatement(std::string& str, std::shared_ptr<Toy> toy);
    bool parseMathematicalOperator(std::string& str, std::shared_ptr<Toy> toy, std::shared_ptr<Toy> first);

    void printToy(std::shared_ptr<Toy> toy, bool last, std::string pre);

  public:
    Parser();
    Parser(std::string _filename);

    void Parse(std::string str);
    std::string GenerateGLSL();
    std::string GetTextureFilePath() const;

    void PrintAST();
};