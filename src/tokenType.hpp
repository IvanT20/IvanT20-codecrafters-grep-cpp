#ifndef TOKEN_TYPE_H
#define TOKEN_TYPE_H

#include <string>

enum class TokenType
{
    Literal,
    Digit,
    Word,
    PosGroup,
    NegGroup,
    StartLineAnchor,
    EndLineAnchor
};

struct Token
{
    TokenType type;
    char value;
};

#endif