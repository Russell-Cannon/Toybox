#include "Parser.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char* argv[]) {
    std::ifstream fin;
    std::string filename;

    bool printAST = false;
    bool printST = false;

    if (argc > 1) {
        filename = argv[1];
        if (argc > 2) {
            for (int i = 2; i < argc; i++) {
                if (strcmp(argv[i], "-printast") == 0 || strcmp(argv[i], "-ast") == 0)
                    printAST = true;
                if (strcmp(argv[i], "-printst") == 0 || strcmp(argv[i], "-st") == 0)
                    printST = true;
            }
        }
    } else {
        std::cout << "Enter file name: ";
        std::cin >> filename;
    }
    fin.open(filename);

    std::stringstream buffer;
    buffer << fin.rdbuf();
    fin.close();
    Parser parser(filename);

    parser.Parse(buffer.str());
    if (printST)
        parser.SymbolTable.Print();
    if (printAST)
        parser.PrintAST();

    std::ifstream scaffold("in.html");
    std::ofstream fout("out.html");
    char c;
    while (scaffold.get(c)) {
        if (c == '$' && scaffold.peek() == '$') { // Read '$$' symbol
            scaffold.ignore(1);                   // Ignore the second $
            std::string command;
            do {
                scaffold.get(c);
                command += c;
            } while (c != '$');

            if (command == "output$") {
                fout << parser.GenerateGLSL();
            } else if (command == "texture$") {
                for (std::string p : parser.SymbolTable.GetTexturePaths()) {
                    fout << "createTexture(\"" << p << "\");\n";
                }
            } else if (command == "uniformtextures$") {
                for (std::string n : parser.SymbolTable.GetTextureNames()) {
                    fout << "uniform sampler2D u_texture_" << n << ";\n";
                }
            } else if (command == "bindtextures$") {
                int i = 0;
                for (std::string n : parser.SymbolTable.GetTextureNames()) {
                    fout << "let iTextureLoc" << n << " = gl.getUniformLocation(prog, \"u_texture_" << n << "\");\n";
                    fout << "gl.uniform1i(iTextureLoc" << n << ", " << i << ");\n";
                    i++;
                }
            }

            scaffold.get(c); // get next character after closing $
        }
        fout << c;
    }
    scaffold.close();

    return 0;
}
