

#include <string> 
#include <iostream> 
#include <unistd.h> 
#include <cstdlib> 
#include "parser.hpp"
#include "process.hpp"

#define PATH_MAX 200
#define HOST_NAME_MAX 100

int main() { 

    const char* username = getenv("USER");
    //Hostname
    char hostname[HOST_NAME_MAX];
    bool got_host_name = false; 
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        got_host_name = true; 
    }

    std::string line; 
    while (true) { 
        if (username) { 
            std::cout << username << '@';
        }
        if (got_host_name) {
            std::cout << hostname << ':'; 
        }
        //cwd
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != nullptr) { 
            std::cout << cwd; 
        }
        std::cout << "$ ";

        std::getline(std::cin, line);
        std::vector<std::string> args = parse_args(line);
        execute_command(args);
    }


    return 0; 
}
