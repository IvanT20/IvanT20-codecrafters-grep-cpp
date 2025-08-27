#include <iostream>
#include <string>

#include "patternMatcher.hpp"

int main(int argc, char* argv[]) 
{
    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // You can use print statements as follows for debugging, they'll be visible when running tests.
    std::cerr << "Logs from your program will appear here" << std::endl;

    if (argc != 3) 
    {
        std::cerr << "Expected two arguments" << std::endl;
        return 1;
    }
    
    std::string flag = argv[1];
    std::string pattern = argv[2];

    if (flag != "-E") 
    {
        std::cerr << "Expected first argument to be '-E'" << std::endl;
        return 1;
    }
    
    std::string input_line;
    std::getline(std::cin, input_line);
    
    PatternMatcher patternMatcher(pattern);

    try 
    {
        bool matched = patternMatcher.match(input_line);
        
        if (matched)
        {
            std::cout << 0 << std::endl;
            return 0;
        }
        else
        {   
            std::cout << 1 << std::endl;
            return 1;
        }
    } 
    catch (const std::runtime_error& e) 
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
