
#include "command.hpp"
#include <unistd.h>

std::string wave_program::cd(const std::vector<std::string>& args) { 
    /* Change directory command */ 
    if (args.size() > 2) { 
        return "Error: Too many arguments, expected one\n";
    }
    if (args.size() < 2) { 
        return "Error: Expected one argument\n";
    }
    if (chdir(args.at(1).c_str()) != 0) { 
        return "Error: Could not change directory to " + args.at(1) + '\n';
    }
    return "";
}







