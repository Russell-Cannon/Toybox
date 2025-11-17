#include "Parser.h"
#include "Toy.h"
#include <iostream>
#include <memory>

Parser::Parser()
    : filename("undefined") {
    document = std::shared_ptr<Toy>(new Document());
}
Parser::Parser(std::string _filename)
    : filename(_filename) {
    document = std::shared_ptr<Toy>(new Document());
}

bool Parser::syntaxError(std::string message) {
    std::cerr << filename << ':' << lineCount << " : " << message << '\n';
    return false;
}

std::string Parser::GetTextureFilePath() const {
    return TexturePath;
}

void Parser::Parse(std::string str) {
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
                syntaxError("Attempted to assign something other than a filepath (" + lex.PeekNext(str).ToString() + ") to a variable. No support yet.");
            }
            // #TODO put logic here for reading functions or variables
            // #TODO check for program deliminator to allow for multiple assignments
        } // else: fall back to ParseStatement
    }

    // start parsing statement
    parseStatement(str, document);
}

std::string Parser::GenerateGLSL() {
    return document->GenerateGLSL();
}

template <typename ToyType>
bool Parser::parseNAryOperation(std::string& str, std::shared_ptr<Toy> toy, int arguments) {
    Lexer lex;
    std::shared_ptr<Toy> typedToy = std::shared_ptr<Toy>(new ToyType());
    if (lex.GetNext(str, lineCount).type != OPEN_PARENTHESIS)
        return syntaxError("Expected opening parenthesis in " + typedToy->Name() + " operation");
    for (int i = 0; i < arguments; i++) {
        if (!parseStatement(str, typedToy))
            return syntaxError("Expected argument #" + std::to_string(i + 1) + " in " + typedToy->Name() + " operation");
        if (i < arguments - 1) { // not the last arguments
            TokenType next = lex.GetNext(str, lineCount).type;
            if (next != COMMA)
                return syntaxError("Expected comma after argument #" + std::to_string(i + 1) + " in " + typedToy->Name() + " operation. Received: " + next.name + " instead.");
        }
    }

    Token next = lex.GetNext(str, lineCount);
    if (next.type != CLOSE_PARENTHESIS)
        return syntaxError("Expected closing parenthesis in " + typedToy->Name() + " operation. Found " + next.ToString() + " instead.");
    toy->AddChild(typedToy);
    return true;
}
template <typename ToyType>
bool Parser::parseUnaryOperation(std::string& str, std::shared_ptr<Toy> toy) {
    return parseNAryOperation<ToyType>(str, toy, 1);
}
template <typename ToyType>
bool Parser::parseBinaryOperation(std::string& str, std::shared_ptr<Toy> toy) {
    return parseNAryOperation<ToyType>(str, toy, 2);
}
template <typename ToyType>
bool Parser::parseTrinaryOperation(std::string& str, std::shared_ptr<Toy> toy) {
    return parseNAryOperation<ToyType>(str, toy, 3);
}
template <typename ToyType>
bool Parser::parseExpandingSizeOperation(std::string& str, std::shared_ptr<Toy> toy) {
    Lexer lex;
    std::shared_ptr<Toy> typedToy = std::shared_ptr<Toy>(new ToyType());
    if (lex.GetNext(str, lineCount).type != OPEN_PARENTHESIS)
        return syntaxError("Expected opening parenthesis in " + typedToy->Name() + " operation");
    // at least one argument is required
    if (!parseStatement(str, typedToy))
        return syntaxError("Expected argument in " + typedToy->Name() + " operation");

    while (lex.PeekNext(str).type == COMMA) {
        lex.GetNext(str, lineCount); // consume comma
        if (!parseStatement(str, typedToy))
            return syntaxError("Expected argument after comma in " + typedToy->Name() + " operation");
    }
    if (lex.GetNext(str, lineCount).type != CLOSE_PARENTHESIS)
        return syntaxError("Expected closing parenthesis in " + typedToy->Name() + " operation");
    toy->AddChild(typedToy);
    return true;
}

bool Parser::parseToy(std::string& str, std::shared_ptr<Toy> toy) {
    Lexer lex;
    Token next = lex.GetNext(str, lineCount);
    if (next.type == IDENTIFIER) {
        if (next.value == "uv") {
            toy->AddChild(std::shared_ptr<Toy>(new LitUV()));
            return true;
        } else if (next.value == "time") {
            toy->AddChild(std::shared_ptr<Toy>(new LitTime()));
            return true;
        } else if (next.value == "frac" || next.value == "fract" || next.value == "fraction" || next.value == "remainder") {
            return parseUnaryOperation<Fract>(str, toy);
        } else if (next.value == "saw" || next.value == "sawtooth" || next.value == "mirror") {
            return parseUnaryOperation<SawTooth>(str, toy);
        } else if (next.value == "sin") {
            return parseUnaryOperation<Sin>(str, toy);
        } else if (next.value == "cos") {
            return parseUnaryOperation<Cos>(str, toy);
        } else if (next.value == "tan") {
            return parseUnaryOperation<Tan>(str, toy);
        } else if (next.value == "normalize" || next.value == "normalized" || next.value == "normal") {
            return parseUnaryOperation<Normalize>(str, toy);
        } else if (next.value == "length" || next.value == "magnitude" || next.value == "len") {
            return parseUnaryOperation<Length>(str, toy);
        } else if (next.value == "text" || next.value == "texture") {
            return parseUnaryOperation<Texture>(str, toy);
        } else if (next.value == "ceil" || next.value == "ceiling") {
            return parseUnaryOperation<Ceiling>(str, toy);
        } else if (next.value == "floor") {
            return parseUnaryOperation<Floor>(str, toy);
        } else if (next.value == "round") {
            return parseUnaryOperation<Round>(str, toy);
        } else if (next.value == "neg" || next.value == "negate") {
            return parseUnaryOperation<Negate>(str, toy);
        } else if (next.value == "abs" || next.value == "absolute") {
            return parseUnaryOperation<Absolute>(str, toy);
        } else if (next.value == "x") {
            return parseUnaryOperation<X>(str, toy);
        } else if (next.value == "y") {
            return parseUnaryOperation<Y>(str, toy);
        } else if (next.value == "z") {
            return parseUnaryOperation<Z>(str, toy);
        } else if (next.value == "screen") {
            return parseUnaryOperation<Screen>(str, toy);
        } else if (next.value == "random" || next.value == "rand") {
            return parseUnaryOperation<Random>(str, toy);
        } else if (next.value == "noise") {
            return parseUnaryOperation<Noise>(str, toy);
        }

        else if (next.value == "mul" || next.value == "mult" || next.value == "multiply") {
            return parseBinaryOperation<Multiply>(str, toy);
        } else if (next.value == "div" || next.value == "divide") {
            return parseBinaryOperation<Divide>(str, toy);
        } else if (next.value == "add") {
            return parseBinaryOperation<Add>(str, toy);
        } else if (next.value == "sub" || next.value == "subtract") {
            return parseBinaryOperation<Subtract>(str, toy);
        } else if (next.value == "mod" || next.value == "modulus" || next.value == "modulo") {
            return parseBinaryOperation<Modulus>(str, toy);
        } else if (next.value == "step" || next.value == "if") {
            return parseBinaryOperation<Step>(str, toy);
        } else if (next.value == "polygon" || next.value == "poly") {
            return parseBinaryOperation<NGon>(str, toy);
        } else if (next.value == "dot") {
            return parseBinaryOperation<Dot>(str, toy);
        } else if (next.value == "cross") {
            return parseBinaryOperation<Cross>(str, toy);
        } else if (next.value == "distance" || next.value == "dist") {
            return parseBinaryOperation<Distance>(str, toy);
        } else if (next.value == "direction" || next.value == "dir") {
            return parseBinaryOperation<Direction>(str, toy);
        } else if (next.value == "circle") {
            return parseBinaryOperation<Circle>(str, toy);
        }

        else if (next.value == "com" || next.value == "comb" || next.value == "combine") {
            return parseTrinaryOperation<Combine>(str, toy);
        } else if (next.value == "clamp") {
            return parseTrinaryOperation<Clamp>(str, toy);
        } else if (next.value == "mix" || next.value == "lerp") {
            return parseTrinaryOperation<Mix>(str, toy);
        } else if (next.value == "smoothstep") {
            return parseTrinaryOperation<SmoothStep>(str, toy);
        } else if (next.value == "line") {
            return parseTrinaryOperation<Line>(str, toy);
        } else if (next.value == "linesegment" || next.value == "segment") {
            return parseTrinaryOperation<LineSegment>(str, toy);
        } else if (next.value == "avg" || next.value == "average") {
            return parseExpandingSizeOperation<Average>(str, toy);
        } else
            return syntaxError("Unknown identifier: " + next.value);
    } else if (next.type == NUMBER) {
        float number = std::atof(next.value.c_str());
        std::shared_ptr<LitNumber> litNum = std::shared_ptr<LitNumber>(new LitNumber());
        litNum->SetNumber(number);
        toy->AddChild(litNum);
        return true;
    }
    if (next.type == OPEN_PARENTHESIS) { // empty parenthesis are legal
        // In the case of ()
        if (lex.PeekNext(str).type == CLOSE_PARENTHESIS)
            return false;
        if (!parseStatement(str, toy))
            return false;
        if (lex.GetNext(str, lineCount).type != CLOSE_PARENTHESIS)
            return syntaxError("Expected closing parenthesis after empty opening parenthesis");
        return true;
    } else if (next.type == EOI) {
        return false;
    }
    return syntaxError("Unexpected symbol: " + next.ToString());
}

bool Parser::parseStatement(std::string& str, std::shared_ptr<Toy> toy) {
    Lexer lex;
    std::shared_ptr<Toy> temp = std::shared_ptr<Toy>(new Toy());
    if (!parseToy(str, temp))
        return false;

    // Only peek: we don't always expect a .? but we want one.
    if (lex.PeekNext(str).type == AXIS) {
        std::string axis = lex.GetNext(str, lineCount).value;
        std::shared_ptr<Toy> returnToy;
        if (axis[1] == 'x' || axis[1] == 'r' || axis[1] == 'u')
            returnToy = std::shared_ptr<Toy>(new X());
        else if (axis[1] == 'y' || axis[1] == 'g' || axis[1] == 'v')
            returnToy = std::shared_ptr<Toy>(new Y());
        else if (axis[1] == 'z' || axis[1] == 'b')
            returnToy = std::shared_ptr<Toy>(new Z());
        returnToy->AddChild(temp->GetChild(0));
        toy->AddChild(returnToy);
        return true;
    } else if (lex.PeekNext(str).type == OPERATOR) {
        return parseMathematicalOperator(str, toy, temp);
    }

    toy->AddChild(temp->GetChild(0));
    return true;
}

bool Parser::parseMathematicalOperator(std::string& str, std::shared_ptr<Toy> toy, std::shared_ptr<Toy> first) {
    Lexer lex;

    std::string op = lex.GetNext(str, lineCount).value;
    std::shared_ptr<Toy> returnToy;
    if (op == "+")
        returnToy = std::shared_ptr<Toy>(new Add());
    else if (op == "-")
        returnToy = std::shared_ptr<Toy>(new Subtract());
    else if (op == "*")
        returnToy = std::shared_ptr<Toy>(new Multiply());
    else if (op == "/")
        returnToy = std::shared_ptr<Toy>(new Divide());

    std::shared_ptr<Toy> second = std::shared_ptr<Toy>(new Toy());
    if (!parseStatement(str, second))
        return syntaxError("No operand to the right of " + op);

    returnToy->AddChild(first->GetChild(0));
    returnToy->AddChild(second->GetChild(0));
    toy->AddChild(returnToy);

    return true;
}

void Parser::PrintAST() {
    std::cout << document->Name() << std::endl;
    if (!document->Empty())
        printToy(document->GetChild(0), true, "");
}

void Parser::printToy(std::shared_ptr<Toy> toy, bool last, std::string pre) {
#ifdef _WIN32
    const char BAR = char(179) /*│*/, MIDDLE = char(195) /*├*/, LAST = char(192) /*└*/, DASH = char(196) /*─*/;
#elif __linux__ // WSL wouldn't print Extended ASCII characters right. They might work on an actual Linux operating system, I'm not sure.
    const char BAR = '|' /*│*/, MIDDLE = '>' /*├*/, LAST = 'L' /*└*/, DASH = '-' /*─*/;
#endif
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
        printToy(toy->GetChild(i), i == toy->NumChildren() - 1, pre + ' ' + BAR);
    }
}
