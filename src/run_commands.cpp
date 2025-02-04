
#include "syntax_tree.hpp"
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream> 
#include "process.hpp"


void SyntaxTree::run_commands(const std::shared_ptr<TokenNode>& node) { 

    /* Determines which function to run based on the token type of the node 
     * Will be called recusrivly by these functions unless the token is a COMMAND, 
     * which is the base case */ 

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
        case Token::REDIR_STDOUT: 
            redir_stdout_command(node);
            break; 
        case Token::REDIR_STDIN: 
            redir_stdin_command(node);
            break; 
        case Token::REDIR_APPEND_STDOUT: 
            redir_append_stdout_command(node);
            break; 
        case Token::COMMAND_SEPERATOR: 
            run_seperate_commands(node);
            break; 
    }
}

void SyntaxTree::run_commands() { 
    //Interface function
    run_commands(m_root);
}


void SyntaxTree::run_command(const Command& command) { 

    /* Executes commands, commands are leafs in the syntax tree and will stop the recursion 
     *
     * If the command is a built in shell command, run the program and exit 
     * Otherwise, spawn two processes to complete the command, with the child
     * completing the command and the parent waiting for the child. 
     * If the command is a background process, then the parent will not wait */ 

    if (command.no_command) return; 
    if (command.exit) std::exit(EXIT_SUCCESS);

    if (command.is_wave_program) { //no need to spawn processes for built in program  
        command.execute(); 
        return; 
    }

    Process process = Process::spawn(); 
                         
    if (process.is_child())    
        command.execute(); 

    if (process.is_parent()) { 
        if (command.is_background_process) { 
            std::cout << "Running background process PID: [" << process << "]\n";
        } else { 
            process.wait_for_child(); 
        }
    }
} 

void SyntaxTree::pipe_command(const std::shared_ptr<TokenNode>& root_token) { 
    int pipefd[2]; //two file descriptors 
    if (pipe(pipefd) == -1) {
        std::cerr << "Pipe failed\n";
        std::exit(EXIT_FAILURE);
    }

    // left command (write to pipe)
    auto left_process = Process::spawn(); 
    if (left_process.is_child()) { 
        dup2(pipefd[1], STDOUT_FILENO);  // redirect stdout to pipe write end
        close(pipefd[0]);  // close unused read end
        close(pipefd[1]);  // close duplicate write end
        run_commands(root_token->left); 
        std::exit(EXIT_SUCCESS);
    }

    // right command (read from pipe)
    auto right_process = Process::spawn(); 

    if (right_process.is_child()) { 
        dup2(pipefd[0], STDIN_FILENO);  // redirect stdin to pipe read end
        close(pipefd[1]);  // close unused write end
        close(pipefd[0]);  // close duplicate read end
        run_commands(root_token->right); 
        std::exit(EXIT_SUCCESS);
    }

    // parent process
    close(pipefd[0]);
    close(pipefd[1]);
    left_process.wait_for_child(); 
    right_process.wait_for_child(); 
} 

void SyntaxTree::and_command(const std::shared_ptr<TokenNode>& node) { 
    /* The right node will run commands if the left node exits successfully */ 

    auto left_process = Process::spawn(); 
    if (left_process.is_child()) {
        // Run child normally
        run_commands(node->left);
        std::exit(EXIT_SUCCESS);
    }  

    // Parent, spawn a right child process if the left exits successfully
    left_process.wait_for_child(); 
    if (left_process.exited() && left_process.exited_successfully()) {
        auto right_process= Process::spawn(); 
        if (right_process.is_child()) {
            run_commands(node->right);
            std::exit(EXIT_SUCCESS);
        } else { 
            right_process.wait_for_child(); 
        }
    }
}

void SyntaxTree::or_command(const std::shared_ptr<TokenNode>& node) { 
    /* The right node will run if the left node exits unsuccesfully*/ 

    auto left_process = Process::spawn(); 
    if (left_process.is_child()) {
        //run child normally
        run_commands(node->left);
        std::exit(EXIT_SUCCESS);
    }  

    // Parent, spawn a right child process if the left exits unsuccessfully
    left_process.wait_for_child(); 
    if (left_process.exited() && !left_process.exited_successfully()) {
        auto right_process= Process::spawn(); 
        if (right_process.is_child()) {
            run_commands(node->right);
            std::exit(EXIT_SUCCESS);
        } else { 
            right_process.wait_for_child(); 
        }
    }
}

void SyntaxTree::redir_stdout_command(const std::shared_ptr<TokenNode>& node) { 
    /* Redirect the left node stdout to the right node file */ 

    auto process = Process::spawn(); 
    if (process.is_child()) { 

        std::vector<std::string> input = node->right->command_line; 
        if (input.size() != 1) { 
            std::cout << "Syntax error: \"<\" Needs one input\n";
            std::exit(EXIT_FAILURE);
        }
        std::freopen(input.at(0).c_str(), "w", stdout);
        run_commands(node->left);
        std::fclose(stdout);
        std::exit(EXIT_SUCCESS);
    }

    if (process.is_parent()) { 
        process.wait_for_child(); 
    }
}


void SyntaxTree::redir_stdin_command(const std::shared_ptr<TokenNode>& node) { 
    /* Redirect the right node file into the left node stdin */ 

    auto process = Process::spawn(); 
    if (process.is_child()) { 

        std::vector<std::string> input = node->right->command_line; 
        if (input.size() != 1) { 
            std::cout << "Syntax error: \"<\" Needs one input\n";
            std::exit(EXIT_FAILURE);
        }

        std::freopen(input.at(0).c_str(), "r", stdin);
        run_commands(node->left);
        std::fclose(stdin);
        std::exit(EXIT_SUCCESS);
    }

    if (process.is_parent()) { 
        process.wait_for_child(); 
    }
}

void SyntaxTree::redir_append_stdout_command(const std::shared_ptr<TokenNode>& node) { 
    /* Redirect the left nodes output to the right node file */ 

    auto process = Process::spawn(); 
    if (process.is_child()) { 

        std::vector<std::string> input = node->right->command_line; 
        if (input.size() != 1) { 
            std::cout << "Syntax error: \">>\" Needs one input\n";
            std::exit(EXIT_FAILURE);
        }

        std::freopen(input.at(0).c_str(), "a", stdout);
        run_commands(node->left);
        std::fclose(stdout);
        std::exit(EXIT_SUCCESS);
    }

    if (process.is_parent()) { 
        process.wait_for_child(); 
    }
}

void SyntaxTree::run_seperate_commands(const std::shared_ptr<TokenNode>& node) { 
    /* Run two commands with no condition */ 

    auto left_process = Process::spawn(); 
    if (left_process.is_child()) {
        run_commands(node->left);
        std::exit(EXIT_SUCCESS);
    }  

    if (left_process.is_parent()) { 
        left_process.wait_for_child(); 
        // Left process complete, complete right process
        
        auto right_process = Process::spawn(); 
        if (right_process.is_child()) { 
            run_commands(node->right);
            std::exit(EXIT_SUCCESS);
        }

        if (right_process.is_parent()) { 
            right_process.wait_for_child(); 
        }
    }
}




