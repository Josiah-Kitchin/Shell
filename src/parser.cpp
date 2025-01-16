
#include "parser.hpp"
#include <sstream> 
#include <cstring> 


std::vector<std::string> parse_args(const std::string& line) { 
    /* Take the line from the command line and push each word to a vector */ 

    std::vector<std::string> args; 
    std::stringstream stream(line);
    std::string word; 

    while (stream >> word) { 
        args.push_back(word);
    }
    return args; 
}

bool is_bg_process(const std::vector<std::string>& args) { 
    return !args.empty() && args.back().back() == '&';
}

void remove_bg_process_symbol(std::vector<std::string>& args) { 
    /* Remove the & symbol */ 
    args.back().pop_back();  
}

std::vector<char*> get_cstr_args(const std::vector<std::string>& args) { 
    /* Given a vector of strings, return a vector of c strings (strings are independent of argument string)*/ 

    std::vector<char*> c_args; 
    c_args.reserve(args.size() + 1);
    
    for (const auto& arg : args) { 
        c_args.push_back(strdup(arg.c_str())); 
    }
    c_args.push_back(nullptr);
    return c_args; 
}





