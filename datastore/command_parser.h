
/* 
 * File:   command_parser.h
 * Author: lasse
 *
 * Created on August 10, 2019, 10:23 AM
 */

#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include "span.h"
#include "prepared_statement.h"


namespace interpreting {
    

class command_parser {
public:
    command_parser();
    
    prepared_statement parse(lsl::span& input_buffer) const;
private:
    static const std::string LEGAL_CHARACTERS;
    
    void parse_create_statement(lsl::span& input_buffer, prepared_statement& statement) const;
    void parse_delete_statement(lsl::span& input_buffer, prepared_statement& statement) const;
    void parse_select_statement(lsl::span& input_buffer, prepared_statement& statement) const;
    void parse_implicit_statement(lsl::span& input_buffer, prepared_statement& statement) const;
    
    void parse_arguments(lsl::span& input_buffer, prepared_statement& statement, const std::string& pattern) const;
    
    void parse_name_argument(std::string& arg, prepared_statement& statement) const;
    void parse_value_argument(std::string& arg, prepared_statement& statement) const;
    unsigned int parse_name_argument_characters(const std::string& arg) const;
    bool strip_outer_parenthesis(std::string& arg) const;
    void un_escape_inner_parenthesis(std::string& arg) const;
};

}

#endif /* COMMAND_PARSER_H */

