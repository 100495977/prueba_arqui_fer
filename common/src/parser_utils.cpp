#include "parser_utils.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>

//Dividimos la línea por palabras separadas por espacios
std::vector<std::string> split_line(const std::string& line) {
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    std::string token;

    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

//Verificamos si la línea está vacía o solo blancos/tabs
bool is_empty_line(const std::string& line) {
    return std::all_of(line.begin(), line.end(),
                        [](char c) {return std::isspace(c);});
}

//Imprimimos un mensaje de error estándar
void print_error(const std::string& message) {
    std::cerr << "Error: " << message << std::end1;
}

