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
    std::map<std::string, int> textureTable;                // Maps texture name to ID.
  public:
    bool Exists(std::string key) {
        return IsConstant(key) | IsFunction(key) | IsTexture(key);
    }
    bool IsConstant(std::string key) {
        return constTable.find(key) != constTable.end();
    }
    bool IsFunction(std::string key) {
        return functionTable.find(key) != functionTable.end();
    }
    bool IsTexture(std::string key) {
        return textureTable.find(key) != textureTable.end();
    }
    std::shared_ptr<Toy> GetConstant(std::string key) {
        return constTable[key];
    }
    Function GetFunction(std::string key) {
        return functionTable[key];
    }
    int GetTextureID(std::string key) {
        return textureTable[key];
    }

    void Insert(std::string key, std::shared_ptr<Toy> toy) {
        if (IsConstant(key))
            std::cerr << "Attempted to add " << key << " twice to constant table.\n";
        constTable[key] = toy;
    }
    void Insert(std::string key, Function function) {
        if (IsFunction(key))
            std::cerr << "Attempted to add " << key << " twice to function table.\n";
        functionTable[key] = function;
    }
    void Insert(std::string key, int id) {
        if (IsTexture(key))
            std::cerr << "Attempted to add " << key << " twice to texture table.\n";
        textureTable[key] = id;
    }

    void Remove(std::string key) {
        if (IsConstant(key))
            constTable.erase(key);
        if (IsFunction(key))
            functionTable.erase(key);
        if (IsTexture(key))
            textureTable.erase(key);
    }

    void Print() {
        for (std::pair<std::string, int> t : textureTable) {
            std::cout << t.first << ": " << t.second << '\n';
        }
        for (std::pair<std::string, std::shared_ptr<Toy>> c : constTable) {
            std::cout << c.first << '\n';
            c.second->Print();
        }
        for (std::pair<std::string, Function> f : functionTable) {
            std::cout << f.first << "\n";
            f.second.AST->Print();
        }
    }
};
