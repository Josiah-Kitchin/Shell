

#pragma once
#include <unistd.h> 
#include <iostream> 


class Process { 
/* Wrapper class for spawning and using processes with unix std lib */ 
 
public: 
    static Process spawn(); //spawns child and parent process   
    bool is_child(); 
    bool is_parent(); 
    void wait_for_child(); //for the parent process to wait for the child process to exit 
    bool exited(); 
    bool exited_successfully(); 

    friend std::ostream& operator<<(std::ostream& out, const Process& process);
private: 
    Process(pid_t pid); 
    pid_t m_pid; 
    int m_status; 
};

std::ostream& operator<<(std::ostream& out, const Process& process);



