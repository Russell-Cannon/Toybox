#pragma once

#include "Lexer.h"
#include "Toy.h"
#include "Types.h"
#include <cstdlib>
#include <iostream>
#include <string>

class Parser {
    Document document;
    std::string filename;
    int lineCount = 1;

  public:
    std::string TexturePath = "./checker-map.png";
    Parser()
        : filename("undefined") {
    }
    Parser(std::string _filename)
        : filename(_filename) {
    }

    bool SyntaxError(std::string message) {
        std::cerr << filename << ':' << lineCount << " : " << message << '\n';
        return false;
    }

    void Parse(std::string str) {
        Lexer lex;
        std::string temp = str;
        int tempLinecount = 0;

        Token next = lex.GetNext(temp, tempLinecount);
        // check for equals
        // check for identifier
        if (next.type == IDENTIFIER) {
            // At this point we could either already be in the expression or in an assignment
            if (lex.PeekNext(temp).type == EQUALS) {            // We now know we are in an assignment
                Token identifier = lex.GetNext(str, lineCount); // start reading for real. We know this is an identifier
                lex.GetNext(str, lineCount);                    // We know this will be an EQUALS. Throw it away

                // We have at least one assignment.
                if (lex.PeekNext(str).type == FILEPATH) { // We have a texture assignment
                    // #TODO add support for arbitrary names for files
                    if (identifier.value == "texture") {
                        TexturePath = lex.GetNext(str, lineCount).value;
                    }
                } else {
                    SyntaxError("Attempted to assign something other than a filepath (" + lex.PeekNext(str).ToString() + ") to a variable. No support yet.");
                }
                // #TODO put logic here for reading functions or variables
                // #TODO check for program deliminator to allow for multiple assignments
            } // else: fall back to ParseStatement
        }

        // start parsing statement
        ParseStatement(str, &document);
    }

    std::string GenerateGLSL() {
        return document.GenerateGLSL();
    }

    template <typename ToyType>
    bool ParseNAryOperation(std::string& str, Toy* toy, int arguments) {
        Lexer lex;
        Toy* typedToy = new ToyType();
        if (lex.GetNext(str, lineCount).type != OPEN_PARENTHESIS)
            return SyntaxError("Expected opening parenthesis in " + typedToy->Name() + " operation");
        for (int i = 0; i < arguments; i++) {
            if (!ParseStatement(str, typedToy))
                return SyntaxError("Expected argument #" + std::to_string(i + 1) + " in " + typedToy->Name() + " operation");
            if (i < arguments - 1) { // not the last arguments
                TokenType next = lex.GetNext(str, lineCount).type;
                if (next != COMMA)
                    return SyntaxError("Expected comma after argument #" + std::to_string(i + 1) + " in " + typedToy->Name() + " operation. Received: " + next.name + " instead.");
            }
        }

        Token next = lex.GetNext(str, lineCount);
        if (next.type != CLOSE_PARENTHESIS)
            return SyntaxError("Expected closing parenthesis in " + typedToy->Name() + " operation. Found " + next.ToString() + " instead.");
        toy->AddChild(typedToy);
        return true;
    }
    template <typename ToyType>
    bool ParseUnaryOperation(std::string& str, Toy* toy) {
        return ParseNAryOperation<ToyType>(str, toy, 1);
    }
    template <typename ToyType>
    bool ParseBinaryOperation(std::string& str, Toy* toy) {
        return ParseNAryOperation<ToyType>(str, toy, 2);
    }
    template <typename ToyType>
    bool ParseTrinaryOperation(std::string& str, Toy* toy) {
        return ParseNAryOperation<ToyType>(str, toy, 3);
    }
    template <typename ToyType>
    bool ParseExpandingSizeOperation(std::string& str, Toy* toy) {
        Lexer lex;
        Toy* typedToy = new ToyType();
        if (lex.GetNext(str, lineCount).type != OPEN_PARENTHESIS)
            return SyntaxError("Expected opening parenthesis in " + typedToy->Name() + " operation");
        // at least one argument is required
        if (!ParseStatement(str, typedToy))
            return SyntaxError("Expected argument in " + typedToy->Name() + " operation");

        while (lex.PeekNext(str).type == COMMA) {
            lex.GetNext(str, lineCount); // consume comma
            if (!ParseStatement(str, typedToy))
                return SyntaxError("Expected argument after comma in " + typedToy->Name() + " operation");
        }
        if (lex.GetNext(str, lineCount).type != CLOSE_PARENTHESIS)
            return SyntaxError("Expected closing parenthesis in " + typedToy->Name() + " operation");
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
            } else if (next.value == "saw" || next.value == "sawtooth" || next.value == "mirror") {
                return ParseUnaryOperation<SawTooth>(str, toy);
            } else if (next.value == "sin") {
                return ParseUnaryOperation<Sin>(str, toy);
            } else if (next.value == "cos") {
                return ParseUnaryOperation<Cos>(str, toy);
            } else if (next.value == "tan") {
                return ParseUnaryOperation<Tan>(str, toy);
            } else if (next.value == "normalize" || next.value == "normalized" || next.value == "normal") {
                return ParseUnaryOperation<Normalize>(str, toy);
            } else if (next.value == "length" || next.value == "magnitude" || next.value == "len") {
                return ParseUnaryOperation<Length>(str, toy);
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
            } else if (next.value == "screen") {
                return ParseUnaryOperation<Screen>(str, toy);
            } else if (next.value == "random" || next.value == "rand") {
                return ParseUnaryOperation<Random>(str, toy);
            } else if (next.value == "noise") {
                return ParseUnaryOperation<Noise>(str, toy);
            }

            else if (next.value == "mul" || next.value == "mult" || next.value == "multiply") {
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
            } else if (next.value == "polygon" || next.value == "poly") {
                return ParseBinaryOperation<NGon>(str, toy);
            } else if (next.value == "dot") {
                return ParseBinaryOperation<Dot>(str, toy);
            } else if (next.value == "cross") {
                return ParseBinaryOperation<Cross>(str, toy);
            } else if (next.value == "distance" || next.value == "dist") {
                return ParseBinaryOperation<Distance>(str, toy);
            } else if (next.value == "direction" || next.value == "dir") {
                return ParseBinaryOperation<Direction>(str, toy);
            } else if (next.value == "circle") {
                return ParseBinaryOperation<Circle>(str, toy);
            }

            else if (next.value == "com" || next.value == "comb" || next.value == "combine") {
                return ParseTrinaryOperation<Combine>(str, toy);
            } else if (next.value == "clamp") {
                return ParseTrinaryOperation<Clamp>(str, toy);
            } else if (next.value == "mix" || next.value == "lerp") {
                return ParseTrinaryOperation<Mix>(str, toy);
            } else if (next.value == "smoothstep") {
                return ParseTrinaryOperation<SmoothStep>(str, toy);
            } else if (next.value == "line") {
                return ParseTrinaryOperation<Line>(str, toy);
            } else if (next.value == "linesegment" || next.value == "segment") {
                return ParseTrinaryOperation<LineSegment>(str, toy);
            } else if (next.value == "avg" || next.value == "average") {
                return ParseExpandingSizeOperation<Average>(str, toy);
            } else
                return SyntaxError("Unknown identifier: " + next.value);
        } else if (next.type == NUMBER) {
            float number = std::atof(next.value.c_str());
            LitNumber* litNum = new LitNumber();
            litNum->SetNumber(number);
            toy->AddChild(litNum);
            return litNum;
        }
        if (next.type == OPEN_PARENTHESIS) { // empty parenthesis are legal
            // In the case of ()
            if (lex.PeekNext(str).type == CLOSE_PARENTHESIS)
                return false;
            if (!ParseStatement(str, toy))
                return false;
            if (lex.GetNext(str, lineCount).type != CLOSE_PARENTHESIS)
                return SyntaxError("Expected closing parenthesis after empty opening parenthesis");
            return true;
        } else if (next.type == EOI) {
            return false;
        }
        return SyntaxError("Unexpected symbol: " + next.ToString());
    }

    bool ParseStatement(std::string& str, Toy* toy) {
        Lexer lex;
        Toy* temp = new Toy();
        if (!ParseToy(str, temp))
            return false;

        // Only peek: we don't always expect a .? but we want one.
        if (lex.PeekNext(str).type == AXIS) {
            std::string axis = lex.GetNext(str, lineCount).value;
            Toy* returnToy;
            if (axis[1] == 'x' || axis[1] == 'r' || axis[1] == 'u')
                returnToy = new X();
            else if (axis[1] == 'y' || axis[1] == 'g' || axis[1] == 'v')
                returnToy = new Y();
            else if (axis[1] == 'z' || axis[1] == 'b')
                returnToy = new Z();
            returnToy->AddChild(temp->GetChild(0));
            toy->AddChild(returnToy);
            return true;
        } else if (lex.PeekNext(str).type == OPERATOR) {
            return ParseMathematicalOperator(str, toy, temp);
        }

        toy->AddChild(temp->GetChild(0));
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
        if (!ParseStatement(str, second))
            return SyntaxError("No operand to the right of " + op);

        returnToy->AddChild(first->GetChild(0));
        returnToy->AddChild(second->GetChild(0));
        toy->AddChild(returnToy);

        return true;
    }

    void PrintAST() {
        std::cout << document.Name() << std::endl;
        if (!document.Empty())
            PrintToy(document.GetChild(0));
    }

    void PrintToy(Toy* toy, bool last = true, std::string pre = "") {
        const char BAR = char(179) /*│*/, MIDDLE = char(195) /*├*/, LAST = char(192) /*└*/, DASH = char(196) /*─*/;
        if (pre.empty())
            pre += BAR;

        // if the last character is a bar, override it with MIDDLE or LAST
        if (pre[pre.length() - 1] == BAR) {
            std::cout << pre.substr(0, pre.length() - 1);
        } else
            std::cout << pre;

        if (last) {
            std::cout << LAST;
            pre = pre.substr(0, pre.length() - 1) + ' ';
        } else
            std::cout << MIDDLE;

        std::cout << DASH << toy->Name() << std::endl;

        for (int i = 0; i < toy->NumChildren(); i++) {
            PrintToy(toy->GetChild(i), i == toy->NumChildren() - 1, pre + ' ' + BAR);
        }
    }
};