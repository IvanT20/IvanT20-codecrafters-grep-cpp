#ifndef TOKEN_TYPE_H
#define TOKEN_TYPE_H

#include <string>
#include <array>

enum class TokenType
{
    Literal,
    Digit,
    Word,
    PosGroup,
    NegGroup,
    StartLineAnchor,
    EndLineAnchor,
};

enum class Quantifier
{
    None,
    OneOrMore
};

struct Token
{
    TokenType type;
    char value;
    std::array<bool, 128> charGroup;
    Quantifier quantifier;
};

#endif