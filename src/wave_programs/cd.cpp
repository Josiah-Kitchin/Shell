
#include "command.hpp"
#include <unistd.h>

std::string wave_program::cd(const std::vector<std::string>& args) { 
    /* Change directory command */ 
    if (args.size() > 1) { 
        return "Error: Too many arguments, expected one\n";
    }
    if (args.empty()) { 
        return "Error: Expected one argument\n";
    }
    if (chdir(args.at(0).c_str()) != 0) { 
        return "Error: Could not change directory to " + args.at(1) + '\n';
    }
    return "";
}







