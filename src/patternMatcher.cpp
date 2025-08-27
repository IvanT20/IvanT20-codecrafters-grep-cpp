#include <iostream>
#include <cctype>

#include "patternMatcher.hpp"

PatternMatcher::PatternMatcher(const std::string& pattern)
{
    m_tokens = Tokenizer::tokenize(pattern);
}

bool PatternMatcher::matchPos(const std::string& input, int inputIndex, int tokenIndex)
{   
    if (tokenIndex == m_tokens.size())
    {
        return true;
    }

    auto matchToken = [&](int index) -> bool
    {
        switch (m_tokens[tokenIndex].type)
        {
            case TokenType::Literal:
                return index < input.size() && m_tokens[tokenIndex].value == input[index];
            case TokenType::Digit:
                return index < input.size() && std::isdigit(input[index]);
            case TokenType::Word:
                return index < input.size() && (std::isalnum(input[index]) || input[index] == '_');
            case TokenType::PosGroup:
                return m_tokens[tokenIndex].charGroup[input[index]];
            case TokenType::NegGroup:
                return !m_tokens[tokenIndex].charGroup[input[index]];
            case TokenType::StartLineAnchor:
                return index == 0;
            case TokenType::EndLineAnchor:
                return index == input.size();
            default:
                return false;
        }
    };

    if (((m_tokens[tokenIndex].type == TokenType::StartLineAnchor) || (m_tokens[tokenIndex].type == TokenType::EndLineAnchor))
        && m_tokens[tokenIndex].quantifier != Quantifier::None)
    {   
        throw std::runtime_error("Anchor can't have quantifiers\n");
    }

    if (m_tokens[tokenIndex].quantifier == Quantifier::None)
    {
        if (!matchToken(inputIndex))
        {
            return false;   
        }

        int offset = (m_tokens[tokenIndex].type == TokenType::StartLineAnchor) || (m_tokens[tokenIndex].type == TokenType::EndLineAnchor) ? 0 : 1;
        return matchPos(input, inputIndex + offset, tokenIndex + 1);
    }
    else if (m_tokens[tokenIndex].quantifier == Quantifier::OneOrMore)
    {   
        int minCount = 1;
        int count = 0;
        int i = inputIndex;

        while (i < input.size() && matchToken(i))
        {
            ++i;
            ++count;
        }

        for (int j = count; j >= minCount; --j)
        {
            if (matchPos(input, inputIndex + j, tokenIndex + 1))
            {
                return true;
            }
        }
    }
    else if (m_tokens[tokenIndex].quantifier == Quantifier::ZeroOrOne)
    {
        if (matchPos(input, inputIndex, tokenIndex + 1))
        {
            return true;
        }

        if (inputIndex < input.size() && matchToken(inputIndex))
        {
            if (matchPos(input, inputIndex + 1, tokenIndex + 1))
            {
                return true;
            }
        }
        
        return false;
    }

    return false;
}

bool PatternMatcher::match(const std::string& input)
{   
    if (!m_tokens.empty() && m_tokens[0].type == TokenType::StartLineAnchor)
    {
        return matchPos(input, 0, 0);
    }
    else
    {
        for (int i = 0; i <= input.size(); ++i)
        {
            if (matchPos(input, i, 0))
            {
                return true;
            }
        }
    }
    
    return false;
}

void PatternMatcher::printTokens()
{
    for (auto& ele : m_tokens)
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