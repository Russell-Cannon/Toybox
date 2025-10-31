#pragma once

#include "Lexer.hpp"
#include "Types.hpp"
#include "Toy.hpp"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>

class Parser {
  Document document;
  std::string filename;
  int lineCount = 1;

public:
  Parser() : filename("undefined") {}
  Parser(std::string _filename) : filename(_filename) {}

    bool SyntaxError(std::string message) {
        std::cerr << filename << ':' << lineCount << " : " << message << '\n';
        return false;
    }


  void Parse(std::string str) {
    Parse(str, &document);
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

    template<typename ToyType> bool ParseNAryOperation(std::string& str, Toy* toy, int arguments) {
        Lexer lex;
        if (lex.GetNext(str, lineCount).type != OPEN_PARENTHESIS)
            return SyntaxError("Expected opening parenthesis in " + std::to_string(arguments) + "-ary operation");
        Toy* typedToy = new ToyType();
        for (int i = 0; i < arguments; i++) {
            if (!Parse(str, typedToy))
                return SyntaxError("Expected argument #" + std::to_string(i + 1) + " in " + std::to_string(arguments) + "-ary operation");
            if (i < arguments - 1) { //not the last arguments
                if (lex.GetNext(str, lineCount).type != COMMA)
                    return SyntaxError("Expected comma after argument #" + std::to_string(i + 1) + " in " + std::to_string(arguments) + "-ary operation");
            }
        }
        if (lex.GetNext(str, lineCount).type != CLOSE_PARENTHESIS)
            return SyntaxError("Expected closing parenthesis in " + std::to_string(arguments) + "-ary operation");
        toy->AddChild(typedToy);
        return true;
    }
    template<typename ToyType> bool ParseUnaryOperation(std::string& str, Toy* toy) {
        return ParseNAryOperation<ToyType>(str, toy, 1);
    }
    template<typename ToyType> bool ParseBinaryOperation(std::string& str, Toy* toy) {
        return ParseNAryOperation<ToyType>(str, toy, 2);
    }
    template<typename ToyType> bool ParseTrinaryOperation(std::string& str, Toy* toy) {
        return ParseNAryOperation<ToyType>(str, toy, 3);
    }
    template<typename ToyType> bool ParseExpandingSizeOperation(std::string& str, Toy* toy) {
        Lexer lex;
        if (lex.GetNext(str, lineCount).type != OPEN_PARENTHESIS)
            return SyntaxError("Expected opening parenthesis in expanding size operation");
        Toy* typedToy = new ToyType();
        //at least one argument is required
        if (!Parse(str, typedToy))
            return SyntaxError("Expected argument in expanding size operation");

        while (lex.PeekNext(str).type == COMMA) {
            lex.GetNext(str, lineCount); //consume comma
            if (!Parse(str, typedToy))
                return SyntaxError("Expected argument after comma in expanding size operation");
        }
        if (lex.GetNext(str, lineCount).type != CLOSE_PARENTHESIS)
            return SyntaxError("Expected closing parenthesis in expanding size operation");
        toy->AddChild(typedToy);
        return true;
    }


bool ParseToy(std::string& str, Toy* toy) {
    Lexer lex;
    Token next = lex.GetNext(str, lineCount);
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
        } else if (next.value == "step" || next.value == "if") {
            return ParseBinaryOperation<Step>(str, toy);
        } else if (next.value == "com" || next.value == "comb" || next.value == "combine") {
            return ParseTrinaryOperation<Combine>(str, toy);
        } else if (next.value == "clamp") {
            return ParseTrinaryOperation<Clamp>(str, toy);
        } else if (next.value == "mix" || next.value == "lerp") {
            return ParseTrinaryOperation<Mix>(str, toy);
        } else if (next.value == "smoothstep") {
            return ParseTrinaryOperation<SmoothStep>(str, toy);
        } else if (next.value == "avg" || next.value == "average") {
            return ParseExpandingSizeOperation<Average>(str, toy);
        } else 
            return SyntaxError("Unknown identifier: " + next.value);
    } else if (next.type == NUMBER) {
        float number = std::atof(next.value.c_str());
        LitNumber* litNum = new LitNumber();
        (*litNum).Value = number;
        toy->AddChild(litNum);
        return litNum;
    } if (next.type == OPEN_PARENTHESIS) { //empty parenthesis are legal
        //In the case of ()
        if (lex.PeekNext(str).type == CLOSE_PARENTHESIS)
            return false;
        if (!Parse(str, toy))
            return false;
        if (lex.GetNext(str, lineCount).type != CLOSE_PARENTHESIS)
            return SyntaxError("Expected closing parenthesis after empty opening parenthesis");
        return true;
    } else if (next.type == EOI) {
        return false;
    }
    return SyntaxError("Unexpected symbol: " + next.type.name);
}

  bool Parse(std::string& str, Toy* toy) {
    Lexer lex;
    Toy* temp = new Toy();
    if (!ParseToy(str, temp)) 
        return false;
    
    //Only peek: we don't always expect a .? but we want one.
    if (lex.PeekNext(str).type == AXIS) {
        std::string axis = lex.GetNext(str, lineCount).value;
        Toy* returnToy;
        if (axis[1] == 'x' || axis[1] == 'r' || axis[1] == 'u')
            returnToy = new X();
        else if (axis[1] == 'y' || axis[1] == 'g' || axis[1] == 'v')
            returnToy = new Y();
        else if (axis[1] == 'z' || axis[1] == 'b')
            returnToy = new Z();
        returnToy->AddChild(temp->children[0]);
        toy->AddChild(returnToy);
        return true;
    } else if (lex.PeekNext(str).type == OPERATOR) {
        return ParseMathematicalOperator(str, toy, temp);
    }

    toy->AddChild(temp->children[0]);
    return true;
  }

    bool ParseMathematicalOperator(std::string& str, Toy* toy, Toy* first) {
        Lexer lex;
        
        std::string op = lex.GetNext(str, lineCount).value;
        Toy* returnToy;        
        if (op == "+")
            returnToy = new Add();
        else if (op == "-")
            returnToy = new Subtract();
        else if (op == "*")
            returnToy = new Multiply();
        else if (op == "/")
            returnToy = new Divide();

        Toy* second = new Toy();
        if (!Parse(str, second))            
            return SyntaxError("No operand to the right of " + op);

        returnToy->AddChild(first->children[0]);
        returnToy->AddChild(second->children[0]);
        toy->AddChild(returnToy);

        return true;
    }

};