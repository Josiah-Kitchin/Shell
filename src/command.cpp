
#include "command.hpp"
#include <cstring> 
#include <unistd.h>
#include <iostream> 
#include <cstdlib>

static bool is_bg_process(const std::vector<std::string>& args) { 
    return !args.empty() && (args.back() == "&" || args.back().back() == '&');
}

static void remove_bg_process_symbol(std::vector<std::string>& args) { 
    /* Remove the & symbol */ 
    if (args.back() == "&") {  
        args.pop_back();  
    } else { 
        args.back().pop_back(); 
    }
}

static std::vector<char*> get_cstr_args(const std::vector<std::string>& args) { 
    /* Given a vector of strings, return a vector of c strings (strings are independent of argument string)*/ 

    std::vector<char*> c_args; 
    c_args.reserve(args.size() + 1);
    
    for (const auto& arg : args) { 
        c_args.push_back(strdup(arg.c_str())); 
    }
    c_args.push_back(nullptr);
    return c_args; 
}


Command::Command(const std::string& program, std::vector<std::string> args) : 
    no_command(false), exit(false), is_background_process(false), 
    is_wave_program(false), m_program(program){
    
    if (m_program == "")  
        no_command = true; 

    if (m_program == "exit")  
        exit = true; 
    
    if (is_bg_process(args)) { 
        is_background_process = true; 
        remove_bg_process_symbol(args);
    }
    if (wave_program::mapper.find(program) != wave_program::mapper.end()) { 
        is_wave_program = true; 
        m_wave_program_function = wave_program::mapper[program];
    }

    m_args = std::move(args);
}


void Command::execute() const { 
    if (is_wave_program) { 
        std::cout << m_wave_program_function(m_args);
        return; 
    } 
    
    std::vector<char*> c_args = get_cstr_args(m_args);
    if (execvp(m_program.c_str(), c_args.data()) == -1) { 
        std::cerr << "Command \"" << m_program << "\" not found\n";
        std::exit(EXIT_FAILURE); 
    }
}
