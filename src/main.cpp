

#include <string> 
#include <iostream> 
#include <unistd.h> 
#include <cstdlib> 
#include "process.hpp"
#include "command.hpp"
#include "interface.hpp"

#define PATH_MAX 200
#define HOST_NAME_MAX 100


int main() { 


    std::string line; 
    while (true) { 

        std::cout << get_interface_line(); 

        std::getline(std::cin, line);
        Command command = build_command(line);
        spawn_command_process(command);
    }


    return 0; 
}
