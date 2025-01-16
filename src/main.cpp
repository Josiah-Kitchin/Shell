

#include <string> 
#include <iostream> 
#include "process.hpp"
#include "command.hpp"
#include "interface.hpp"

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
