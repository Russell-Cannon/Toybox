#include "Parser.h"
#include "Toy.h"
#include "Types.h"
#include <iostream>
#include <memory>
#include <typeinfo>
#include <vector>

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
    int _lineCount = 0;
    Token id = lex.GetNext(str, _lineCount);
    if (id.type != IDENTIFIER)
        return false;

    Token next = lex.GetNext(str, _lineCount);
    if (next.type == OPEN_PARENTHESIS) { // if its an open parameter: start reading the parameters
        while (lex.PeekNext(str).type == IDENTIFIER) {
            lex.GetNext(str, _lineCount); // throw that ID
            if (lex.PeekNext(str).type == COMMA)
                lex.GetNext(str, _lineCount); // throw that comma
        }
        if (lex.GetNext(str, _lineCount).type != CLOSE_PARENTHESIS)
            return false;
        return lex.GetNext(str, _lineCount).type == EQUALS;
    }

    return next.type == EQUALS; // if there's no parenthesis: its an constant or texture assignment: return if there is an equals
}

bool Parser::parseAssignment(std::string& str) {
    Lexer lex;
    Token id = lex.GetNext(str, lineCount);

    if (lex.PeekNext(str).type == OPEN_PARENTHESIS) // we have function declaration
        return parseFunctionDeclaration(str, id.value);

    // otherwise:
    lex.GetNext(str, lineCount); //.type == EQUALS

    Token operand = lex.GetNext(str, lineCount);
    if (operand.type == FILEPATH) {
        SymbolTable.Insert(id.value, operand.value);
        return true;
    }
    // Undo stealing that operand
    str = operand.value + ' ' + str;
    std::shared_ptr<Toy> holder = std::shared_ptr<Toy>(new Toy);
    if (parseStatement(str, holder)) {
        if (holder->NumChildren() == 0)
            std::cerr << "Parsed empty statement!";
        SymbolTable.Insert(id.value, holder->GetChild(0));
        return true;
    }

    return semanticError("Expected a valid Toybox statement or filepath after equals. Got: " + operand.ToString() + " instead.");
}

bool Parser::parseFunctionDeclaration(std::string& str, std::string id) {
    Lexer lex;
    lex.GetNext(str, lineCount); // throw open parenthesis
    Function function;

    while (lex.PeekNext(str).type != CLOSE_PARENTHESIS) { // get all parameters
        Token next = lex.GetNext(str, lineCount);
        if (next.type != IDENTIFIER)
            return syntaxError("Expected parameter inside function declaration.");
        function.parameters.push_back(next.value);
        if (lex.PeekNext(str).type == COMMA)
            lex.GetNext(str, lineCount); // throw out the comma
    }

    if (lex.GetNext(str, lineCount).type != CLOSE_PARENTHESIS)
        return syntaxError("Expected closing parenthesis after parameters in function declaration.");

    if (lex.GetNext(str, lineCount).type != EQUALS)
        return syntaxError("Expected equals sign after id and parameters in function declaration.");

    // push our parameters to the symbol table
    for (int i = 0; i < function.parameters.size(); i++) {
        SymbolTable.Insert(function.parameters[i], std::shared_ptr<Toy>(new Parameter(function.parameters[i])));
    }

    // read the expression
    std::shared_ptr<Toy> holder = std::shared_ptr<Toy>(new Toy);
    if (!parseStatement(str, holder))
        return false;

    // remove our parameters
    for (int i = 0; i < function.parameters.size(); i++) {
        SymbolTable.Remove(function.parameters[i]);
    }

    // push the expression to the function table under our id
    function.AST = holder->GetChild(0);
    SymbolTable.Insert(id, function);

    return true;
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

bool Parser::parseFunction(std::string& str, std::string id, std::shared_ptr<Toy> toy) {
    Lexer lex;
    std::vector<std::shared_ptr<Toy>> arguments;
    Function fun = SymbolTable.GetFunction(id);
    // get arguments
    if (lex.GetNext(str, lineCount).type != OPEN_PARENTHESIS)
        return false;
    while (lex.PeekNext(str).type != CLOSE_PARENTHESIS) {
        std::shared_ptr<Toy> holder = std::shared_ptr<Toy>(new Toy);
        if (!parseStatement(str, holder))
            return false;
        arguments.push_back(holder->GetChild(0));
        if (lex.PeekNext(str).type == COMMA)
            lex.GetNext(str, lineCount);
    }
    if (lex.GetNext(str, lineCount).type != CLOSE_PARENTHESIS)
        return false;

    if (arguments.size() != fun.parameters.size())
        return semanticError("Argument list does not match number of parameters in " + id + " function.");

    // go through AST and replace parameters with arguments
    std::shared_ptr<Toy> head = fun.AST->Clone(); // this needs to be a deep copy
    DFSAndReplace(head, head, fun.parameters, arguments);

    // add fixed ast to toy
    toy->AddChild(head);
    return true;
}

void Parser::DFSAndReplace(std::shared_ptr<Toy> head, std::shared_ptr<Toy> last, const std::vector<std::string>& find, const std::vector<std::shared_ptr<Toy>>& replace) {
    // if head is in find
    if (typeid(*head.get()) == typeid(Parameter)) {
        Parameter* param = (Parameter*)head.get();
        for (int i = 0; i < find.size(); i++) {
            if (param->ID == find[i]) {
                int pos = last->RemoveChild(head);
                // make last point to replace
                last->AddChild(replace[i], pos);
                return;
            }
        }
    }
    // else: recurse on all children
    for (int i = 0; i < head->NumChildren(); i++)
        DFSAndReplace(head->GetChild(i), head, find, replace);
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

        } else if (SymbolTable.Exists(next.value)) { // id exists as key in symbol table
            if (SymbolTable.IsTexture(next.value)) {
                if (!parseUnaryOperation<Texture>(str, toy))
                    return false;
                Texture* text = (Texture*)toy->GetChild(0).get(); // get the texture we just added.
                text->SetTexture(next.value);                     // set the texture id
                return true;
            } else if (SymbolTable.IsConstant(next.value)) {
                toy->AddChild(SymbolTable.GetConstant(next.value)); // add constant variable
                return true;
            } else if (SymbolTable.IsFunction(next.value)) { // key represents a function
                return parseFunction(str, next.value, toy);
            }
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
    if (next.type == OPEN_PARENTHESIS) {
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
        returnToy->AddChild(temp->GetChild(0)); // add the inner expression to an X/Y/Z toy to take its dimension.
        // catch cases where an axis is operated on
        if (lex.PeekNext(str).type == OPERATOR)
            return parseMathematicalOperator(str, toy, returnToy);
        else {
            toy->AddChild(returnToy); // Add this new toy (an X/Y/Z with the original expression inside) to the given toy
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

void Parser::PrintAST() {
    std::cout << document->Name() << ":\n";
    if (!document->Empty())
        document->GetChild(0)->Print();
}
