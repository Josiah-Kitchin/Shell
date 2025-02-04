


#include <sys/wait.h>
#include "process.hpp"


Process Process::spawn() { 
    /* Spawn a chlid and parent process 
     * Exits with failure code on failure */  

    pid_t pid = fork(); 
    if (pid == -1) { 
        std::cerr << "Failed to spawn process\n";
        exit(EXIT_FAILURE);
    }
    return Process(pid);
}

Process::Process(pid_t pid) : m_pid(pid) {}

bool Process::is_child() { 
    return m_pid == 0;
}

bool Process::is_parent() { 
    return m_pid > 0;
}

void Process::wait_for_child() { 
    /* Blocks parent process until child exits */ 
    int status; 
    waitpid(m_pid, &status, 0);
    m_status = status; 
}

bool Process::exited() { 
    return WIFEXITED(m_status);
}

bool Process::exited_successfully() { 
    return WEXITSTATUS(m_status) == 0;
}

std::ostream& operator<<(std::ostream& out, const Process& process) { 
    out << process.m_pid; 
    return out; 
}





