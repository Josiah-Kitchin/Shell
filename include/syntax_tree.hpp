
#pragma once

#include <string> 
#include "command.hpp"
#include <memory> 
#include <unordered_map>

/* ---------- Tokens ---------- */ 

enum class Token { 
    COMMAND, 
    PIPE, 
    AND, 
    OR, 
    REDIR_STDOUT, 
    REDIR_STDIN, 
    REDIR_APPEND_STDOUT,
    COMMAND_SEPERATOR
}; 

const std::unordered_map<std::string, Token> token_map = {
    {"|", Token::PIPE},
    {"&&", Token::AND},
    {"||", Token::OR},
    {">", Token::REDIR_STDOUT},
    {"<", Token::REDIR_STDIN},
    {">>", Token::REDIR_APPEND_STDOUT},
    {";", Token::COMMAND_SEPERATOR},
};

Token get_token(const std::string&);

struct TokenNode { 
    TokenNode(Token type); 
    Command get_command(); 

    Token type; 
    std::shared_ptr<TokenNode> left = nullptr; 
    std::shared_ptr<TokenNode> right = nullptr;  

    std::vector<std::string> command_line; 
};

/* ------------------------------- */

class SyntaxTree { 
/* The syntax tree class is responsible for running the proccesses. 
 * The syntax tree is a binary tree holding token nodes. The leafs of the tree are commands, while 
 * the operators are the parents. 
 * This allows for the synax tree to recurisvly run the command leafs and direct proprly 
 * according to the parent operator */ 

public: 
    SyntaxTree(const std::string& command_line); //takes the command line written by the user 
    void run_commands(); //interface functions to run commands at the root 

private: 
    std::shared_ptr<TokenNode> m_root; 
    void run_commands(const std::shared_ptr<TokenNode>& root_token); //recursivly run commands and follow operators 
    void run_command(const Command& command); //execute a single command 
    void pipe_command(const std::shared_ptr<TokenNode>& root_token); //redirect output of left child command to right child command
    void and_command(const std::shared_ptr<TokenNode>& node); //executes right child command if left child was successfull
    void or_command(const std::shared_ptr<TokenNode>& node); //executes right child command if left child was unsuccesfull
    void redir_stdout_command(const std::shared_ptr<TokenNode>& node); //redirect output of left command to a file 
    void redir_stdin_command(const std::shared_ptr<TokenNode>& node); //redirect right file to input of left command 
    void redir_append_stdout_command(const std::shared_ptr<TokenNode>& node); //redirect output of left command to append to a file 
    void run_seperate_commands(const std::shared_ptr<TokenNode>& node); // Run left node and right node with no conditions 

};



