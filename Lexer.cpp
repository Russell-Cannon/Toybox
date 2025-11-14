#include "Lexer.h"
#include "Types.h"

#include <iostream>
#include <regex>

Lexer::Lexer() {}

Token Lexer::GetNext(std::string& str, int& _lineCount) {
    std::smatch match;

    while (str.length() > 0 && (str[0] == ' ' || str[0] == '\n' || str[0] == '\t' || str[0] == 13)) {
        if (str[0] == '\n')
            _lineCount++;
        str = str.substr(1);
    }

    if (str.length() == 0)
        return {EOI, "Empty String"};

    if (std::regex_search(str, match, LINE_COMMENT.pattern)) {
        str = str.substr(match[0].length());
        _lineCount++;
        return GetNext(str, _lineCount); // skip to next token
    }
    if (std::regex_search(str, match, MULTI_LINE_COMMENT.pattern)) {
        for (char c : match[0].str())
            if (c == '\n')
                _lineCount++;

        str = str.substr(match[0].length());
        return GetNext(str, _lineCount); // skip to next token
    }

    DEBUG("Current string: ", str);
    for (TokenType tt : TokenTypes) {
        if (std::regex_search(str, match, tt.pattern)) {
            std::string found = match[0].str();
            std::transform(found.begin(), found.end(), found.begin(), ::tolower);
            DEBUG("Found: ", tt.name);
            str = str.substr(found.length());
            return {tt, found};
        }
    }
    std::cerr << "UNKNOWN CHARACTER: '" << str[0] << "' (" << (int)str[0] << ")\n";
    return {ERROR, str};
}

Token Lexer::PeekNext(std::string& str) {
    int n = 0;
    std::string temp(str);
    return GetNext(temp, n);
}
