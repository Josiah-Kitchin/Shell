
#include "syntax_tree.hpp"
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream> 
#include "process.hpp"


void SyntaxTree::run_commands(const std::shared_ptr<TokenNode>& node) { 
    /* Mages proccesses and executes command, being built in shell commands or binaries */ 
    if (node == nullptr) return; 

    switch (node->type){
        case Token::COMMAND: { 
            Command command = node->get_command(); 
            run_command(command);
            break;
        }
        case Token::PIPE: 
            pipe_command(node); 
            break; 
        case Token::AND: 
            and_command(node);
            break; 
        case Token::OR: 
            or_command(node);
            break;
    }
}

void SyntaxTree::run_commands() { 
    run_commands(m_root);
}


void SyntaxTree::run_command(const Command& command) { 
    if (command.no_command) return; 
    if (command.exit) std::exit(EXIT_SUCCESS);

    if (command.is_wave_program) { 
        command.execute(); 
        return; 
    }
    Process process = Process::spawn(); 
                         
    if (process.is_child()) {  //child process 
        command.execute(); 
    } else if (process.is_parent()) { //parent process
        if (!command.is_background_process) { 
            process.wait_for_child(); 
        } else { 
            std::cout << "Running background process PID: [" << process << "]\n";
        }
    } else { 
        std::cerr << "Fork failed\n"; 
    }
} 

void SyntaxTree::pipe_command(const std::shared_ptr<TokenNode>& root_token) { 
    int pipefd[2]; //two file descriptors 
    if (pipe(pipefd) == -1) {
        std::cerr << "Pipe failed\n";
        exit(EXIT_FAILURE);
    }

    // left command (write to pipe)
    pid_t left_pid = fork(); 
    if (left_pid == -1) {
        std::cerr << "Fork failed\n";
        exit(EXIT_FAILURE);
    }
    if (left_pid == 0) { 
        dup2(pipefd[1], STDOUT_FILENO);  // redirect stdout to pipe write end
        close(pipefd[0]);  // close unused read end
        close(pipefd[1]);  // close duplicate write end
        run_commands(root_token->left); 
        exit(EXIT_SUCCESS);
    }

    // right command (read from pipe)
    pid_t right_pid = fork(); 
    if (right_pid == -1) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    if (right_pid == 0) { 
        dup2(pipefd[0], STDIN_FILENO);  // redirect stdin to pipe read end
        close(pipefd[1]);  // close unused write end
        close(pipefd[0]);  // close duplicate read end
        run_commands(root_token->right); 
        exit(EXIT_SUCCESS);
    }

    // parent process
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(left_pid, nullptr, 0);
    waitpid(right_pid, nullptr, 0);
} 

void SyntaxTree::and_command(const std::shared_ptr<TokenNode>& node) { 
    /* The right node will run if the left node exits successfully */ 

    auto left_process = Process::spawn(); 
    if (left_process.is_child()) {
        //run child normally
        run_commands(node->left);
        exit(EXIT_SUCCESS);
    } else { 
        //parent, spawn a right child process if the left exits successfully
        left_process.wait_for_child(); 
        if (left_process.exited() && left_process.exited_successfully()) {
            auto right_process= Process::spawn(); 
            if (right_process.is_child()) {
                run_commands(node->right);
                exit(EXIT_SUCCESS);
            } else { 
                right_process.wait_for_child(); 
            }
        }
    }
}

void SyntaxTree::or_command(const std::shared_ptr<TokenNode>& node) { 
    /* The right node will run if the left node exits unsuccesfully*/ 

    auto left_process = Process::spawn(); 
    if (left_process.is_child()) {
        //run child normally
        run_commands(node->left);
        exit(EXIT_SUCCESS);
    } else { 
        //parent, spawn a right child process if the left exits successfully
        left_process.wait_for_child(); 
        if (left_process.exited() && !left_process.exited_successfully()) {
            auto right_process= Process::spawn(); 
            if (right_process.is_child()) {
                run_commands(node->right);
                exit(EXIT_SUCCESS);
            } else { 
                right_process.wait_for_child(); 
            }
        }
    }
}

