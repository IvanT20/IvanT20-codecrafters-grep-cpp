#include <stdexcept>

#include "tokenizer.hpp"

std::vector<Token> Tokenizer::tokenize(const std::string& pattern)
{
    std::vector<Token> tokens;

    for (int i = 0; i < pattern.length(); ++i)
    {
        Token token;
        token.quantifier = Quantifier::None;

        if (pattern[i] == '[')
        {
            ++i;
            token.type = (i < pattern.length() && pattern[i] == '^') ? TokenType::NegGroup : TokenType::PosGroup;

            if (token.type == TokenType::NegGroup)
            {
                ++i;
            }

            while (i < pattern.length() && pattern[i] != ']')
            {
                token.charGroup[pattern[i]] = true;
                ++i;
            }

            if (i >= pattern.length() && pattern[i] != ']')
            {
                throw std::runtime_error("Missing closing bracket: " + pattern);
            }
            tokens.push_back(token);
            continue; // To go past the ] for the next character
        }
        else if (pattern[i] == '\\' && i + 1 < pattern.length())
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
            }
        }
        else if (pattern[i] == '^')
        {
            token.type = TokenType::StartLineAnchor;
            tokens.push_back(token);
            continue;
        }
        else if (pattern[i] == '$')
        {
            token.type = TokenType::EndLineAnchor;
            tokens.push_back(token);
            continue;
        }
        else if (pattern[i] == '+')
        {
            tokens.back().quantifier = Quantifier::OneOrMore;
            continue;
        }
        else if (pattern[i] == '?')
        {
            tokens.back().quantifier = Quantifier::ZeroOrOne;
            continue;
        }
        else if (pattern[i] == '.')
        {
            token.type = TokenType::AnyCharacter;
            tokens.push_back(token);
            continue;
        }

        token.type = TokenType::Literal;
        token.value = pattern[i];
        tokens.push_back(token);
    }

    return tokens;
}