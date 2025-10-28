#pragma once

#include "Lexer.hpp"
#include "Types.hpp"
#include "Toy.hpp"
#include <cstdlib>
#include <iostream>
#include <fstream>

class Parser {
  Document document;

public:
  Parser() {}

  void Parse(std::string str) {
    ParseToy(str, &document);
  }

  void GenerateGLSL(std::string outputFileName) {
    std::ifstream preamble("preamble.txt");
    std::ifstream postamble("postamble.txt");
    std::ofstream fout(outputFileName);

    fout << preamble.rdbuf();
    fout << document.GenerateGLSL();
    fout << postamble.rdbuf();

    preamble.close();
    fout.close();
    postamble.close();
  }

  bool ParseToy(std::string& str, Toy* toy) {
    Lexer lex;
    Token next = lex.GetNext(str);
    if (next.type == IDENTIFIER) {
        if (next.value == "uv") {
            toy->AddChild(new LitUV());
            return true;
        } else if (next.value == "time") {
            toy->AddChild(new LitTime());
            return true;
        } else if (next.value == "fract" || next.value == "fraction" || next.value == "remainder") {
            if (lex.GetNext(str).type != OPEN_PARENTHESIS)
                return false;
            Toy* fract = new Fract();
            if (!ParseToy(str, fract))  //if true: fract is given a child
                return false;
            if (lex.GetNext(str).type != CLOSE_PARENTHESIS)
                return false;
            toy->AddChild(fract);
            return true;
        } else if (next.value == "mult" || next.value == "multiply") {
            if (lex.GetNext(str).type != OPEN_PARENTHESIS)
                return false;

            Toy* mult = new Multiply();
            if (!ParseToy(str, mult))
                return false;
            if (lex.GetNext(str).type != COMMA)
                return false;
            if (!ParseToy(str, mult))
                return false;

            if (lex.GetNext(str).type != CLOSE_PARENTHESIS)
                return false;

            toy->AddChild(mult);
            return true;
        }
    } else if (next.type == NUMBER) {
        float number = std::atof(next.value.c_str());
        LitNumber* litNum = new LitNumber();
        (*litNum).Value = number;
        toy->AddChild(litNum);
        return litNum;
    }
    return false;
  }
};