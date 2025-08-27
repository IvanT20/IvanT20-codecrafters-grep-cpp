#ifndef PATTERN_MATCHER_H
#define PATTERN_MATCHER_H

#include <string>
#include <vector>

#include "tokenizer.hpp"

class PatternMatcher
{

public:

    PatternMatcher(const std::string& pattern);
    bool matchPos(const std::string& input, int inputIndex, int tokenIndex);
    bool match(const std::string& input);
    void printTokens();
    
private:

    std::vector<Token> m_tokens;
};

#endif