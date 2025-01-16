
#include "syntax_tree.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <iostream> 



void SyntaxTree::run_proccesses() { 
    /* Mages proccesses and executes command, being built in shell commands or binarys */ 
    if (m_root == nullptr) return; 

    switch (m_root ->type){
        case Token::COMMAND: { 
            Command command = m_root->get_command(); 
            spawn_command_process(command);
            break;
        }
        case Token::PIPE: 
            pipe_command(m_root); 
            break; 
    }
}

void SyntaxTree::run_proccesses(const std::shared_ptr<TokenNode>& node) { 
    /* Mages proccesses and executes command, being built in shell commands or binarys */ 
    if (node == nullptr) return; 

    switch (node->type){
        case Token::COMMAND: { 
            Command command = node->get_command(); 
            spawn_command_process(command);
            break;
        }
        case Token::PIPE: 
            pipe_command(node); 
            break; 
    }
}

void SyntaxTree::spawn_command_process(const Command& command) { 
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

void SyntaxTree::pipe_command(const std::shared_ptr<TokenNode>& root_token) { 
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe failed");
        exit(EXIT_FAILURE);
    }

    // Left command (write to pipe)
    pid_t left_pid = fork(); 
    if (left_pid == -1) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    if (left_pid == 0) { 
        dup2(pipefd[1], STDOUT_FILENO);  // Redirect stdout to pipe write end
        close(pipefd[0]);  // Close unused read end
        close(pipefd[1]);  // Close duplicate write end
        run_proccesses(root_token->left);  // Execute left command
        exit(EXIT_FAILURE);
    }

    // Right command (read from pipe)
    pid_t right_pid = fork(); 
    if (right_pid == -1) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    if (right_pid == 0) { 
        dup2(pipefd[0], STDIN_FILENO);  // Redirect stdin to pipe read end
        close(pipefd[1]);  // Close unused write end
        close(pipefd[0]);  // Close duplicate read end
        run_proccesses(root_token->right);  // Execute right command
        exit(EXIT_FAILURE);
    }

    // Parent process
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(left_pid, nullptr, 0);
    waitpid(right_pid, nullptr, 0);
} 

