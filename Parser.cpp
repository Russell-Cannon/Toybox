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
bool Parser::semanticError(std::string message) {
    std::cerr << filename << ':' << lineCount << " : " << message << '\n';
    return false;
}

std::string Parser::GetTextureFilePath(int i) const {
    return texturePaths[i];
}

int Parser::GetNumTextures() const {
    return texturePaths.size();
}

void Parser::AddTexture(std::string _filename) {
    if (GetNumTextures() == 8) {
        semanticError("Attempted to add a ninth texture. GLSL only supports eight or less textures.");
        return;
    }
    texturePaths.push_back(_filename);
}

void Parser::Parse(std::string str) {
    Lexer lex;
    std::string temp = str;
    int tempLinecount = 0;

    while (isAssignment(str)) {
        parseAssignment(str);
    }

    // start parsing statement
    parseStatement(str, document);
}

bool Parser::isAssignment(std::string str) {
    Lexer lex;
    Token id = lex.GetNext(str, lineCount);
    if (id.type != IDENTIFIER)
        return false;

    if (lex.GetNext(str, lineCount).type != EQUALS)
        return false;

    //could assign a quilt or a filepath
    // Token operand = lex.GetNext(str, lineCount);
    // if (operand.type == FILEPATH)
        return true;
    // return syntaxError("Attempted to assign something other than a filepath (" + operand.ToString() + ") to a variable. No support yet.");
}

bool Parser::parseAssignment(std::string& str) {
    Lexer lex;
    Token id = lex.GetNext(str, lineCount);
    lex.GetNext(str, lineCount); //.type == EQUALS

    Token operand = lex.GetNext(str, lineCount);
    if (operand.type == FILEPATH) {
        if (id.value == "texture") { //redefining texture globally
            texturePaths[0] = operand.value;
        } else { //assigning a texture to variable
            //create a new texture toy
            Texture* t = new Texture();
            t->SetTextureID(GetNumTextures()); //set its id to this end of the list
            symbolTable[id.value] = {true, std::shared_ptr<Toy>(t)};

            //add that texture to the list
            AddTexture(operand.value);
        }
        return true;
    }
    //Undo stealing that operand
    str = operand.value + ' ' + str;
    std::shared_ptr<Toy> holder = std::shared_ptr<Toy>(new Toy);
    if (parseStatement(str, holder)) {
        symbolTable[id.value] = {false, holder->GetChild(0)};
        return true;
    }

    return semanticError("Expected a valid Toybox statement or filepath after equals. Got: " + operand.ToString() + " instead.");
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
        } else if (next.value == "fract") {
            return parseUnaryOperation<Fract>(str, toy);
        } else if (next.value == "mirror") {
            return parseUnaryOperation<SawTooth>(str, toy);
        } else if (next.value == "sin") {
            return parseUnaryOperation<Sin>(str, toy);
        } else if (next.value == "cos") {
            return parseUnaryOperation<Cos>(str, toy);
        } else if (next.value == "tan") {
            return parseUnaryOperation<Tan>(str, toy);
        } else if (next.value == "normalize") {
            return parseUnaryOperation<Normalize>(str, toy);
        } else if (next.value == "length") {
            return parseUnaryOperation<Length>(str, toy);
        } else if (next.value == "texture") {
            return parseUnaryOperation<Texture>(str, toy);
        } else if (next.value == "ceil") {
            return parseUnaryOperation<Ceiling>(str, toy);
        } else if (next.value == "floor") {
            return parseUnaryOperation<Floor>(str, toy);
        } else if (next.value == "round") {
            return parseUnaryOperation<Round>(str, toy);
        } else if (next.value == "abs") {
            return parseUnaryOperation<Absolute>(str, toy);
        } else if (next.value == "negate") {
            return parseUnaryOperation<Negate>(str, toy);
        } else if (next.value == "screen") {
            return parseUnaryOperation<Screen>(str, toy);
        } else if (next.value == "random") {
            return parseUnaryOperation<Random>(str, toy);
        } else if (next.value == "noise") {
            return parseUnaryOperation<Noise>(str, toy);

        } else if (next.value == "mod") {
            return parseBinaryOperation<Modulus>(str, toy);
        } else if (next.value == "step") {
            return parseBinaryOperation<Step>(str, toy);
        } else if (next.value == "polygon") {
            return parseBinaryOperation<NGon>(str, toy);
        } else if (next.value == "dot") {
            return parseBinaryOperation<Dot>(str, toy);
        } else if (next.value == "cross") {
            return parseBinaryOperation<Cross>(str, toy);
        } else if (next.value == "distance") {
            return parseBinaryOperation<Distance>(str, toy);
        } else if (next.value == "direction") {
            return parseBinaryOperation<Direction>(str, toy);
        } else if (next.value == "circle") {
            return parseBinaryOperation<Circle>(str, toy);
        }

        else if (next.value == "combine") {
            return parseTrinaryOperation<Combine>(str, toy);
        } else if (next.value == "clamp") {
            return parseTrinaryOperation<Clamp>(str, toy);
        } else if (next.value == "mix") {
            return parseTrinaryOperation<Mix>(str, toy);
        } else if (next.value == "smoothstep") {
            return parseTrinaryOperation<SmoothStep>(str, toy);
        } else if (next.value == "line") {
            return parseTrinaryOperation<Line>(str, toy);
        } else if (next.value == "linesegment") {
            return parseTrinaryOperation<LineSegment>(str, toy);
        } else if (next.value == "average") {
            return parseExpandingSizeOperation<Average>(str, toy);
        
        } else if (symbolTable.find(next.value) != symbolTable.end()) { //id exists as key in symbol table
            bool function = symbolTable[next.value].first;
            if (function) {
                parseUnaryOperation<Texture>(str, toy);
                Texture* text = (Texture*)toy->GetChild(0).get(); //get the texture we just added.
                Texture* symbolTexture = (Texture*)symbolTable[next.value].second.get(); //get the texture from the symbol table.
                text->SetTextureID(symbolTexture->GetTextureID()); //set the texture id in the new texture to the id from the old one.
            } else {
                toy->AddChild(symbolTable[next.value].second); //add constant variable
            }
            return true;
        } else {
            return syntaxError("Unknown identifier: " + next.value);
        }
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
        else if (axis[1] == 'z' || axis[1] == 'b' || axis[1] == 'w')
            returnToy = std::shared_ptr<Toy>(new Z());
        returnToy->AddChild(temp->GetChild(0)); //add the inner expression to an X/Y/Z toy to take its dimension.
        //catch cases where an axis is operated on
        if (lex.PeekNext(str).type == OPERATOR)
            return parseMathematicalOperator(str, toy, returnToy); //
        else {
            toy->AddChild(returnToy); //Add this new toy (an X/Y/Z with the original expression inside) to the given toy
            return true; 
        }
    } else if (lex.PeekNext(str).type == OPERATOR) {
        return parseMathematicalOperator(str, toy, temp->GetChild(0));
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

    returnToy->AddChild(first);
    returnToy->AddChild(second->GetChild(0));
    toy->AddChild(returnToy);

    return true;
}

void Parser::PrintSymbolTable() {
    for (std::pair<std::string, std::pair<bool, std::shared_ptr<Toy>>> p : symbolTable) {
        std::cout << p.first << std::endl;
        printToy(p.second.second, true, "");
    }
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
