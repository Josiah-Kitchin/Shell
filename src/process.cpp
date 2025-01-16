
#include "process.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include "parser.hpp"
#include <iostream> 
#include "commands.hpp"

void execute_command(std::vector<std::string> args) { 
    /* Mages proccesses and executes command, being built in shell commands or binarys */ 
    
    if (args.empty()) return; 
    if (args.at(0) == "exit") exit(EXIT_SUCCESS);

    // Execute built in commands, no need to start a process 
    if (command::COMMAND_MAPPER.find(args.at(0)) != command::COMMAND_MAPPER.end()) { 
        auto command = command::COMMAND_MAPPER[args.at(0)];
        std::string result = command(args);
        std::cout << result; 
        return; 
    }

    bool background_process = is_bg_process(args);
    if (background_process)  
        remove_bg_process_symbol(args);
    
    pid_t pid = fork();  //spawn the parent and child process
                         
    if (pid == 0) {  //child process 
        std::vector<char*> c_args = get_cstr_args(args);
        if (execvp(c_args.at(0), c_args.data()) == -1) { 
            std::cerr << "Command \"" << c_args.at(0) << "\" not found\n";
        }
    } else if (pid > 0) { //parent process
        if (!background_process)
            waitpid(pid, nullptr, 0);
        else { 
            std::cout << "Running background process PID: [" << pid << "]\n";
        }
    } else { 
        std::cerr << "Fork failed\n"; 
    }
}
