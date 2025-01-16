
#include "process.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <iostream> 

void spawn_command_process(const Command& command) { 
    /* Mages proccesses and executes command, being built in shell commands or binarys */ 
    
    if (command.no_command) return; 
    if (command.exit) std::exit(EXIT_SUCCESS);

    if (command.is_wave_program) { 
        command.execute(); 
        return; 
    }
    
    pid_t pid = fork();  //spawn the parent and child process
                         
    if (pid == 0) {  //child process 
        command.execute(); 
    } else if (pid > 0) { //parent process
        if (!command.is_background_process) { 
            waitpid(pid, nullptr, 0);
        } else { 
            std::cout << "Running background process PID: [" << pid << "]\n";
        }
    } else { 
        std::cerr << "Fork failed\n"; 
    }
}
