

#include <sstream> 
#include <cassert> 
#include "syntax_tree.hpp"
#include <iostream> 


std::vector<std::string> tokenize(const std::string &line) { 
    /* Transform the string into a vector of tokens */ 
    std::vector<std::string> words; 
    std::stringstream stream(line);
    std::string word; 

    while (stream >> word) { 
        words.push_back(word);
    }

    return words; 
}

TokenNode::TokenNode(Token type) : type(type) {}
Command TokenNode::get_command() { 
    assert(type == Token::COMMAND);
    
    if (command_line.empty()) { 
        std::string program = "";
        return Command(program, command_line);
    }
    std::string program = command_line.at(0);
    return Command(program, command_line);
}

std::shared_ptr<TokenNode> build_syntax_tree(const std::vector<std::string>& tokens) { 
    /* Build a syntax tree, with operators being the parents and commands being the leafs. */ 

    auto command_node = std::make_shared<TokenNode>(Token::COMMAND); 
    for (size_t i = 0; i < tokens.size(); ++i) { 
        if (tokens[i] == op::PIPE) { 
            auto pipe_node = std::make_shared<TokenNode>(Token::PIPE);
            pipe_node->left = command_node; 
            pipe_node->right = build_syntax_tree(std::vector<std::string>(tokens.begin() + i + 1, tokens.end()));
            return pipe_node; 
        } else if (tokens[i] == op::AND) { 
            auto and_node = std::make_shared<TokenNode>(Token::AND);
            and_node->left = command_node; 
            and_node->right = build_syntax_tree(std::vector<std::string>(tokens.begin() + i + 1, tokens.end()));
            return and_node; 
        } else if (tokens[i] == op::OR) { 
            auto or_node = std::make_shared<TokenNode>(Token::OR);
            or_node->left = command_node; 
            or_node->right = build_syntax_tree(std::vector<std::string>(tokens.begin() + i + 1, tokens.end()));
            return or_node;
        } 
        else { 
            command_node->command_line.push_back(tokens[i]);
        }
    }
    return command_node; 
}

SyntaxTree::SyntaxTree(const std::string& command_line) { 
    std::vector<std::string> tokens = tokenize(command_line);
    m_root = build_syntax_tree(tokens);
}


