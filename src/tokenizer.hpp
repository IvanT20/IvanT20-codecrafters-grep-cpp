#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <stdexcept>
#include <cctype>
#include <vector>
#include <array>
#include <iostream>

#include "tokenType.hpp"

struct Tokenizer
{
    Tokenizer(const std::string& input, const std::string& pattern);
    void tokenize(const std::string& pattern);
    bool matchPosition(int pos);
    bool matchToken(Token& token, char c);
    bool lineTest(bool checkStart);
    bool match();
    void print();
    std::vector<Token> tokens;
    std::array<bool, 128> charGroup;
    std::string value;
};


#endif
