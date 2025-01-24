

#include <sstream> 
#include <cassert> 
#include "syntax_tree.hpp"


Token get_token(const std::string& input) { 
    auto it = token_map.find(input);
    if (it != token_map.end()) { 
        return it->second; 
    }
    return Token::COMMAND; 
}

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
        Token type = get_token(tokens[i]);
        if (type != Token::COMMAND) { 
            auto operator_node = std::make_shared<TokenNode>(type);
            operator_node->left = command_node; 
            operator_node->right = build_syntax_tree(std::vector<std::string>(tokens.begin() + i + 1, tokens.end()));
            return operator_node; 
        } 
        command_node->command_line.push_back(tokens[i]);
    }
    return command_node; 
}

SyntaxTree::SyntaxTree(const std::string& command_line) { 
    std::vector<std::string> tokens = tokenize(command_line);
    m_root = build_syntax_tree(tokens);
}


