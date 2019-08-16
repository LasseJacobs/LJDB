
/* 
 * File:   command_parser.h
 * Author: lasse
 *
 * Created on August 10, 2019, 10:23 AM
 */

#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H


#include <queue>
#include <map>

#include "prepared_statement.h"
#include "text_iterator.h"
#include "tokenizer.h"


namespace eval {
    
enum class token_class_t {
    COMMAND,
    SPECIFIER,
    ARGUMENT
};
    

class command_parser {
public:
    command_parser();
    
    prepared_statement parse(std::queue<token_t> tokens, const std::map<std::string, std::string>& context) const;
private:
    static const std::string LEGAL_CHARACTERS;
    
    void parse_create_statement(std::queue<token_t>& tokens, const std::map<std::string, std::string>& context, prepared_statement& statement) const;
    void parse_delete_statement(std::queue<token_t>& tokens, const std::map<std::string, std::string>& context, prepared_statement& statement) const;
    void parse_select_statement(std::queue<token_t>& tokens, const std::map<std::string, std::string>& context, prepared_statement& statement) const;
    
    void parse_arguments(std::queue<token_t>& tokens, const std::map<std::string, std::string>& context, prepared_statement& statement, const std::string& pattern) const;
    
    void parse_name_argument(const token_t& arg, prepared_statement& statement) const;
    void parse_value_argument(const token_t& arg, prepared_statement& statement) const;
    void get_context(const std::map<std::string, std::string>& context, prepared_statement& statement) const;
    
    int find_illegal_characters(const std::string& arg) const;
    bool strip_outer_parenthesis(std::string& arg) const;
    void un_escape_inner_parenthesis(std::string& arg) const;
};

}

#endif /* COMMAND_PARSER_H */

