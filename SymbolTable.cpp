#include "SymbolTable.h"
#include "Toy.h"
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

SymbolTable::SymbolTable() {
    textureTable["texture"] = "./checker-map.png";
}

bool SymbolTable::Exists(std::string key) {
    return IsConstant(key) | IsFunction(key) | IsTexture(key);
}
bool SymbolTable::IsConstant(std::string key) {
    return constTable.find(key) != constTable.end();
}
bool SymbolTable::IsFunction(std::string key) {
    return functionTable.find(key) != functionTable.end();
}
bool SymbolTable::IsTexture(std::string key) {
    return textureTable.find(key) != textureTable.end();
}
std::shared_ptr<Toy> SymbolTable::GetConstant(std::string key) {
    return constTable[key];
}
Function SymbolTable::GetFunction(std::string key) {
    return functionTable[key];
}
std::string SymbolTable::GetTexture(std::string key) {
    return textureTable[key];
}

void SymbolTable::Insert(std::string key, std::shared_ptr<Toy> toy) {
    if (IsConstant(key))
        std::cerr << "Attempted to add " << key << " twice to constant table.\n";
    constTable[key] = toy;
}
void SymbolTable::Insert(std::string key, Function function) {
    if (IsFunction(key))
        std::cerr << "Attempted to add " << key << " twice to function table.\n";
    functionTable[key] = function;
}
void SymbolTable::Insert(std::string key, std::string texture) {
    if (key != "texture" && IsTexture(key))
        std::cerr << "Attempted to add " << key << " twice to texture table.\n";
    textureTable[key] = texture;
}

void SymbolTable::Remove(std::string key) {
    if (IsConstant(key))
        constTable.erase(key);
    if (IsFunction(key))
        functionTable.erase(key);
    if (IsTexture(key))
        textureTable.erase(key);
}

std::vector<std::string> SymbolTable::GetTextureNames() {
    std::vector<std::string> names;
    for (auto i : textureTable)
        names.insert(names.begin(), i.first);
    return names;
}
std::vector<std::string> SymbolTable::GetTexturePaths() {
    std::vector<std::string> paths;
    for (auto i : textureTable)
        paths.insert(paths.begin(), i.second);
    return paths;
}

void SymbolTable::Print() {
    for (std::pair<std::string, std::string> t : textureTable) {
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
