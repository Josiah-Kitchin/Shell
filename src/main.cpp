

#include <string> 
#include <iostream> 
#include "syntax_tree.hpp"
#include "interface.hpp"

int main() { 

    std::string line; 
    while (true) { 
        std::cout << get_interface_line(); 
        std::getline(std::cin, line);
        SyntaxTree tree(line);
        tree.run_proccesses(); 
    }

    return 0; 
}
