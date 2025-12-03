#include "Toy.h"
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

class Function {
  public:
    std::vector<std::string> parameters;
    std::shared_ptr<Toy> AST;
};

class SymbolTable {
  protected:
    std::map<std::string, std::shared_ptr<Toy>> constTable; // Maps constant variable name to shape
    std::map<std::string, Function> functionTable;          // Maps function name to shape and parameters
    std::map<std::string, std::string> textureTable;        // Maps texture name to filepath.

  public:
    SymbolTable();

    bool Exists(std::string key);
    bool IsConstant(std::string key);
    bool IsFunction(std::string key);
    bool IsTexture(std::string key);
    std::shared_ptr<Toy> GetConstant(std::string key);
    Function GetFunction(std::string key);
    std::string GetTexture(std::string key);
    void Insert(std::string key, std::shared_ptr<Toy> toy);
    void Insert(std::string key, Function function);
    void Insert(std::string key, std::string texture);
    void Remove(std::string key);
    std::vector<std::string> GetTextureNames();
    std::vector<std::string> GetTexturePaths();
    void Print();
};
