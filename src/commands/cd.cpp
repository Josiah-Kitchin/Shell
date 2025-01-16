
#include "commands.hpp"
#include <unistd.h>

std::string command::cd(const std::vector<std::string>& args) { 
    /* Change directory command */ 
    if (args.size() > 2) { 
        return "Error: Too many arguments, expected one";
    }
    if (args.size() < 2) { 
        return "Error: expected one argument";
    }
    if (chdir(args.at(1).c_str()) != 0) { 
        return "Error: Could not change directory to " + args.at(1) + '\n';
    }
    return "";
}







