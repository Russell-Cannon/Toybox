#pragma once

#include "Lexer.h"
#include "SymbolTable.h"
#include "Toy.h"
#include "Types.h"
#include <map>
#include <memory>
#include <string>

class Parser {
    std::shared_ptr<Toy> document;
    SymbolTable symbolTable;
    std::string filename = "";
    int lineCount = 1;
    std::vector<std::string> texturePaths = {"./checker-map.png"};

    bool syntaxError(std::string message);
    bool semanticError(std::string message);

    void DFSAndReplace(std::shared_ptr<Toy> head, std::shared_ptr<Toy> last, const std::vector<std::string>& find, const std::vector<std::shared_ptr<Toy>>& replace);

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
    bool isAssignment(std::string str);
    bool parseAssignment(std::string& str);
    bool parseFunctionDeclaration(std::string& str, std::string id);
    bool parseFunction(std::string& str, std::string id, std::shared_ptr<Toy> toy);
    bool parseMathematicalOperator(std::string& str, std::shared_ptr<Toy> toy, std::shared_ptr<Toy> first);

  public:
    Parser();
    Parser(std::string _filename);

    void Parse(std::string str);
    std::string GenerateGLSL();
    std::string GetTextureFilePath(int i) const;
    int GetNumTextures() const;
    void AddTexture(std::string _filename);

    void PrintAST();
    void PrintSymbolTable();
};