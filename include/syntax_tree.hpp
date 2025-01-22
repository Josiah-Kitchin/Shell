
#pragma once

#include <string> 
#include "command.hpp"
#include <memory> 

enum class Token { COMMAND, PIPE, AND, OR }; 

namespace op { 
    const std::string PIPE = "|";
    const std::string AND = "&&";
    const std::string OR = "||";
}

struct TokenNode { 
    TokenNode(Token type); 
    Command get_command(); 

    Token type; 
    std::shared_ptr<TokenNode> left = nullptr; 
    std::shared_ptr<TokenNode> right = nullptr;  

    std::vector<std::string> command_line; 
};

class SyntaxTree { 
/* The syntax tree class is responsible for running the proccesses. 
 * The syntax tree is a binary tree holding token nodes. The leafs of the tree are commands, while 
 * the operators are the parents. 
 * This allows for the synax tree to recurisvly run the command leafs and direct proprly 
 * according to the parent operator */ 

public: 
    SyntaxTree(const std::string& command_line);
    void run_commands(); 

private: 
    std::shared_ptr<TokenNode> m_root; 
    void run_commands(const std::shared_ptr<TokenNode>& root_token); 
    void run_command(const Command& command);
    void pipe_command(const std::shared_ptr<TokenNode>& root_token);
    void and_command(const std::shared_ptr<TokenNode>& node);
    void or_command(const std::shared_ptr<TokenNode>& node);

};



