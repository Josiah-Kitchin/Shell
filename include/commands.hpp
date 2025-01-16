

#pragma once

#include <functional> 
#include <string> 
#include <unordered_map> 
#include <vector> 


namespace command {
    std::string cd(const std::vector<std::string>& args); 
    std::string exit(const std::vector<std::string>& args);

    static std::unordered_map<std::string, std::function<std::string(const std::vector<std::string>&)>> COMMAND_MAPPER {
        {"cd", cd}
    };
}
