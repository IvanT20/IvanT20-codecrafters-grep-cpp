#include "tokenizer.hpp"

Tokenizer::Tokenizer(const std::string& input, const std::string& pattern)
:value(input)
{
    tokenize(pattern);
}

void Tokenizer::tokenize(const std::string& pattern)
{
    for (int i = 0; i < pattern.length(); i++)
    {
        Token token;
        if (pattern[i] == '[')
        {
            ++i;
            token.type = (i < pattern.length() && pattern[i] == '^') ? TokenType::NegGroup : TokenType::PosGroup;

            while (i < pattern.length() && pattern[i] != ']')
            {
                charGroup[pattern[i]] = true;
                ++i;
            }

            if (i < pattern.length() && pattern[i] != ']')
            {
                throw std::runtime_error("Missing closing bracket: " + pattern);
            }
            tokens.push_back(token);
            continue; // To go past the ] for the next character
        }
        if (pattern[i] == '\\' && i + 1 < pattern.length())
        {
            char charRegex = pattern[i + 1];

            switch (charRegex)
            {
                case 'd':
                    token.type = TokenType::Digit;
                    tokens.push_back(token);
                    ++i;
                    continue;
                
                case 'w':
                    token.type = TokenType::Word;
                    tokens.push_back(token);
                    ++i;
                    continue;

                default:
                    throw std::runtime_error("Unhandled character pattern: " + pattern);
            }
        }

        token.type = TokenType::Literal;
        token.value = pattern[i];
        tokens.push_back(token);
    }
}

bool Tokenizer::matchToken(Token& token, char c)
{   
    switch (token.type)
    {
        case TokenType::Literal:
            return c == token.value;
        case TokenType::Digit:
            return std::isdigit(c);
        case TokenType::Word:
            return std::isalnum(c) || c == '_';
        case TokenType::PosGroup:
            return charGroup[c];
        case TokenType::NegGroup:
            return !charGroup[c];
        default:
            return false;
    }
}

bool Tokenizer::matchPosition(int pos)
{
    if (pos + tokens.size() > value.length())
    {
        return false;
    }

    for (int i = 0; i < tokens.size(); i++)
    {
        if (!matchToken(tokens[i], value[i + pos]))
        {
            return false;
        }
    }

    return true;
}

bool Tokenizer::match()
{   
    for (int i = 0; i < value.length(); i++)
    {
        if (matchPosition(i))
        {
            return true;
        }
    }

    return false;
}