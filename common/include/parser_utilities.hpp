#ifndef PARSER_UTILITIES_HPP
#define PARSER_UTILITIES_HPP

#include <string>
#include <vector>

std::vector<std::string> split_line(const std::string &line);
bool is_empty_line(const std::string &line);
void print_error(const std::string &message, int line_number);


#endif // PARSER_UTILITIES_HPP