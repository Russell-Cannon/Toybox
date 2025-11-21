#include "Parser.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char* argv[]) {
    std::ifstream fin;
    std::string filename;

    if (argc > 1) {
        filename = argv[1];
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
    parser.PrintSymbolTable();
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
                for (std::pair<std::string, std::string> t : parser.GetTextures()) {
                    fout << "createTexture(\"" << t.second << "\");\n";
                }
            } else if (command == "uniformtextures$") {
                for (std::pair<std::string, std::string> t : parser.GetTextures()) {
                    fout << "uniform sampler2D u_texture_" << t.first << ";\n";
                }
            } else if (command == "bindtextures$") {
                int i = 0;
                for (std::pair<std::string, std::string> t : parser.GetTextures()) {
                    fout << "let iTextureLoc" << t.first << " = gl.getUniformLocation(prog, \"u_texture_" << t.first << "\");\n";
                    fout << "gl.uniform1i(iTextureLoc" << t.first << ", " << i << ");\n";
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
