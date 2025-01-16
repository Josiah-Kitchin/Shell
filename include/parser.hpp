
#pragma once

#include <vector> 
#include <string> 

std::vector<std::string> parse_args(const std::string& line); 
std::vector<char*> get_cstr_args(const std::vector<std::string>& args);
bool is_bg_process(const std::vector<std::string>& args);
void remove_bg_process_symbol(std::vector<std::string>& args);




