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

    template<typename ToyType> bool ParseUnaryOperation(std::string& str, Toy* toy) {
        Lexer lex;
        if (lex.GetNext(str).type != OPEN_PARENTHESIS) {
            std::cerr << "Expected opening parenthesis in unary operation";
            return false;
        }
        Toy* typedToy = new ToyType();
        if (!ParseToy(str, typedToy)) {
            std::cerr << "Expected argument in unary operation";
            return false;
        }
        if (lex.GetNext(str).type != CLOSE_PARENTHESIS) {
            std::cerr << "Expected closing parenthesis in unary operation";
            return false;
        }
        toy->AddChild(typedToy);
        return true;
    }

    template<typename ToyType> bool ParseBinaryOperation(std::string& str, Toy* toy) {
        Lexer lex;
        if (lex.GetNext(str).type != OPEN_PARENTHESIS) {
            std::cerr << "Expected opening parenthesis in binary operation";
            return false;
        }

        Toy* typedToy = new ToyType();
        if (!ParseToy(str, typedToy)) {
            std::cerr << "Expected argument in binary operation";
            return false;
        }
        if (lex.GetNext(str).type != COMMA) {
            std::cerr << "Expected comma after first argument in binary operation";
            return false;
        }
        if (!ParseToy(str, typedToy)) {
            std::cerr << "Expected second argument in binary operation";
            return false;
        }

        if (lex.GetNext(str).type != CLOSE_PARENTHESIS) {
            std::cerr << "Expected closing parenthesis in binary operation";
            return false;
        }

        toy->AddChild(typedToy);
        return true;
    }

    template<typename ToyType> bool ParseTrinaryOperation(std::string& str, Toy* toy) {
        Lexer lex;
        if (lex.GetNext(str).type != OPEN_PARENTHESIS) {
            std::cerr << "Expected opening parenthesis in trinary operation";
            return false;
        }

        Toy* typedToy = new ToyType();
        if (!ParseToy(str, typedToy)) {
            std::cerr << "Expected argument in trinary operation";
            return false;
        }
        if (lex.GetNext(str).type != COMMA) {
            std::cerr << "Expected comma after first argument in trinary operation";
            return false;
        }
        if (!ParseToy(str, typedToy)) {
            std::cerr << "Expected second argument in trinary operation";
            return false;
        }
        if (lex.GetNext(str).type != COMMA) {
            std::cerr << "Expected comma after second argument in trinary operation";
            return false;
        }
        if (!ParseToy(str, typedToy)) {
            std::cerr << "Expected third argument in trinary operation";
            return false;
        }

        if (lex.GetNext(str).type != CLOSE_PARENTHESIS) {
            std::cerr << "Expected closing parenthesis in trinary operation";
            return false;
        }

        toy->AddChild(typedToy);
        return true;
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
        } else if (next.value == "frac" || next.value == "fract" || next.value == "fraction" || next.value == "remainder") {
            return ParseUnaryOperation<Fract>(str, toy);
        } else if (next.value == "sin") {
            return ParseUnaryOperation<Sin>(str, toy);
        } else if (next.value == "cos") {
            return ParseUnaryOperation<Cos>(str, toy);
        } else if (next.value == "tan") {
            return ParseUnaryOperation<Tan>(str, toy);
        } else if (next.value == "text" || next.value == "texture") {
            return ParseUnaryOperation<Texture>(str, toy);
        } else if (next.value == "ceil" || next.value == "ceiling") {
            return ParseUnaryOperation<Ceiling>(str, toy);
        } else if (next.value == "floor") {
            return ParseUnaryOperation<Floor>(str, toy);
        } else if (next.value == "round") {
            return ParseUnaryOperation<Round>(str, toy);
        } else if (next.value == "neg" || next.value == "negate") {
            return ParseUnaryOperation<Negate>(str, toy);
        } else if (next.value == "abs" || next.value == "absolute") {
            return ParseUnaryOperation<Absolute>(str, toy);
        } else if (next.value == "x") {
            return ParseUnaryOperation<X>(str, toy);
        } else if (next.value == "y") {
            return ParseUnaryOperation<Y>(str, toy);
        } else if (next.value == "z") {
            return ParseUnaryOperation<Z>(str, toy);
        } else if (next.value == "mul" || next.value == "mult" || next.value == "multiply") {
            return ParseBinaryOperation<Multiply>(str, toy);
        } else if (next.value == "div" || next.value == "divide") {
            return ParseBinaryOperation<Divide>(str, toy);
        } else if (next.value == "add") {
            return ParseBinaryOperation<Add>(str, toy);
        } else if (next.value == "sub" || next.value == "subtract") {
            return ParseBinaryOperation<Subtract>(str, toy);
        } else if (next.value == "mod" || next.value == "modulus" || next.value == "modulo") {
            return ParseBinaryOperation<Modulus>(str, toy);
        } else if (next.value == "com" || next.value == "comb" || next.value == "combine") {
            return ParseTrinaryOperation<Combine>(str, toy);
        } else if (next.value == "clamp") {
            return ParseTrinaryOperation<Clamp>(str, toy);
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