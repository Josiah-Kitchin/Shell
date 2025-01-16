
#include "interface.hpp"
#include <unistd.h> 
#include <string> 

constexpr int HOST_NAME_MAX = 100; 
constexpr int PATH_MAX = 100; 

std::string get_interface_line() {
    std::string interface_line = "";

    const char* username = getenv("USER");
    if (username) { 
        (interface_line += username) += "@";
    }
    char hostname[HOST_NAME_MAX];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        (interface_line += hostname) += ":";
    }
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) { 
        interface_line += cwd;
    }
    interface_line += "$ ";
    return interface_line; 
}

