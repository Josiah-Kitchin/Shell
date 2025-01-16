

#pragma once

#include <functional> 
#include <string> 
#include <unordered_map> 
#include <vector> 

namespace wave_program {
    /* Built in wave programs 
     * Take in a vector of strings for the argument and return the result string */

    std::string cd(const std::vector<std::string>& args); 

    //to be set anytime a new built in program is created, able to map a program name to a function 
    static std::unordered_map<std::string, std::function<std::string(const std::vector<std::string>&)>> mapper {
        {"cd", wave_program::cd}
    };
}

class Command {
    /* Stores commands metadata and provides the execute method to run a command, built-in or system. 
     * Commands built from an input string with build_command */ 
public: 
    Command(const std::string& program, std::vector<std::string> args);
    //meta data the process spawner needs to know 
    bool no_command; //empty program string
    bool exit;  //if the command was to exit 
    bool is_background_process; //& symbol at the end of the arguments 
    bool is_wave_program;  //if the program to run is a built in wave program 

    void execute() const;

private: 
    std::string m_program;
    std::function<std::string(const std::vector<std::string>&)> m_wave_program_function; 
    std::vector<std::string> m_args; 

};

Command build_command(const std::string& line);




