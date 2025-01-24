
#pragma once

#include <string> 
#include "command.hpp"
#include <memory> 
#include <unordered_map>

/* ---------- Tokens ---------- */ 

enum class Token { COMMAND, PIPE, AND, OR, OUTPUT_RIGHT, OUTPUT_LEFT, APPEND_RIGHT}; 

const std::unordered_map<std::string, Token> token_map = {
    {"|", Token::PIPE},
    {"&&", Token::AND},
    {"||", Token::OR},
    {">", Token::OUTPUT_RIGHT},
    {"<", Token::OUTPUT_LEFT},
    {">>", Token::APPEND_RIGHT},
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
    /*void output_right_command(const std::shared_ptr<TokenNode>& node);*/
    /*void output_left_command(const std::shared_ptr<TokenNode>& node);*/
    /*void append_right_command(const std::shared_ptr<TokenNode>& node);*/

};



