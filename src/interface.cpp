
#include "interface.hpp"
#include <unistd.h> 
#include <string> 

constexpr int HOST_NAME_MAX = 100; 
constexpr int PATH_MAX = 100; 

std::string get_interface_line() {
    std::string interface_line = "\033[39;1m";

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

        std::string path(cwd);
        const std::string homePrefix = "/home/";
        
        if (path.find(homePrefix) == 0) {
            size_t next_slash= path.find('/', homePrefix.length());
            if (next_slash != std::string::npos) {
                interface_line += "~" + path.substr(next_slash);
            } else {
                interface_line += "~";
            }
        }
    }
    interface_line += "\x1b[34m$ ";
    return interface_line; 
}

