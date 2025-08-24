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

            if (token.type == TokenType::NegGroup)
            {
                ++i;
            }

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

bool Tokenizer::lineTest()
{   
    if (tokens.size() == 1)
    {
        return true;
    }

    bool hasStart = tokens[0].type == TokenType::StartLineAnchor;
    bool hasEnd = tokens[tokens.size() - 1].type == TokenType::EndLineAnchor;
    int startIndex = hasStart ? 1 : 0;
    int endIndex = hasEnd ? tokens.size() - 2 : tokens.size() - 1;
    int patternLength = endIndex - startIndex + 1;

    if (hasStart && hasEnd)
    {
        if (patternLength != value.length())
        {
            return false;
        }

        for (int i = 0; i < patternLength; i++)
        {
            if (!matchToken(tokens[startIndex + i], value[i]))
            {
                return false;
            }
        }
        return true;
    }
    else if (hasStart)
    {   
        for (int i = 0; i < patternLength; i++)
        {
            if (!matchToken(tokens[startIndex + i], value[i]))
            {
                return false;
            }
        }
        return true;
    }
    else 
    {
        for (int i = endIndex, j = value.size() - 1; i >= 0; i--, j--)
        {
            if (!matchToken(tokens[i], value[j]))
                return false;
        }
        return true;
    }
}


bool Tokenizer::match()
{   
    if (tokens[0].type == TokenType::StartLineAnchor || (tokens.size() > 0 && tokens[tokens.size() - 1].type == TokenType::EndLineAnchor))
    {
        return lineTest();
    }

    for (int i = 0; i < value.length(); i++)
    {
        if (matchPosition(i))
        {
            return true;
        }
    }

    return false;
}

void Tokenizer::print()
{
    for (auto& ele : tokens)
    {
        switch (ele.type)
        {

            case TokenType::Literal:
                std::cout << "LITERAL, ";
                break;
            
            case TokenType::Digit:
                std::cout << "DIGIT, ";
                break;
            
            case TokenType::Word:
                std::cout << "WORD, ";
                break;
            
            case TokenType::PosGroup:
                std::cout << "POS_GROUP, ";
                break;
            
            case TokenType::NegGroup:
                std::cout << "NEG_GROUP, ";
                break;
            
            case TokenType::StartLineAnchor:
                std::cout << "START_LINE_ANCHOR, ";
                break;
            
            case TokenType::EndLineAnchor:
                std::cout << "END_LINE_ANCHOR, ";
                break;

            default:
                break;
        }
    }
    std::cout << '\n';
}