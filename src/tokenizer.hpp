#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>

#include "tokenType.hpp"

struct Tokenizer
{
    Tokenizer() = delete;
    static std::vector<Token> tokenize(const std::string& pattern);
};

#endif
