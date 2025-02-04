
#include "config.hpp"
#include <nlohmann/json.hpp>
#include <fstream> 
#include <iostream> 
#include <cstdlib> 

UserConfig::UserConfig() { 
    // Default config 
    line_color = "\033[39;1m";
    text_color = "\x1b[34m$ ";
}

UserConfig get_config() { 
    //Look for .wave_config.json in the home dir 
    //and return the config struct 
    const char* homeDir = std::getenv("HOME");
    if (!homeDir) { 
        std::cerr << "Home directory not found\n";
        std::exit(EXIT_FAILURE);
    }

    std::ifstream config_file(std::string(homeDir) + "/.wave_config.json");
    if (!config_file) { 
        std::cerr << "Could not file wave config\n"; 
        std::exit(EXIT_FAILURE);
    }

    nlohmann::json config_data; 
    config_file >> config_data; 

    UserConfig config; 
    std::string line_color = config_data["line_color"];
    std::string text_color = config_data["text_color"];

    if (COLOR_MAP.find(line_color) != COLOR_MAP.end()) { 
        config.line_color = COLOR_MAP.at(line_color);
    }
    if (COLOR_MAP.find(text_color) != COLOR_MAP.end()) { 
        config.text_color = COLOR_MAP.at(text_color);
    }
    return config; 
}

