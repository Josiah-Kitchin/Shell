

#include "command.hpp"
#include <sstream> 


Command build_command(const std::string &line) { 
    std::vector<std::string> words; 
    std::stringstream stream(line);
    std::string word; 

    while (stream >> word) { 
        words.push_back(word);
    }

    if (words.empty()) { 
        std::string program = "";
        return Command(program, words);
    }
    std::string program = words.at(0);
    return Command(program, words);
}
