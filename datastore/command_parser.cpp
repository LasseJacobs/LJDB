
/* 
 * File:   command_parser.cpp
 * Author: lasse
 * 
 * Created on August 10, 2019, 10:23 AM
 */

#include "command_parser.h"
#include "utils.h"
#include <assert.h>     /* assert */

namespace interpreting {
    
    enum class gtoken_t {
        EXIT,
        CREATE,
        DELETE,
        SELECT,
        IMPLICIT
    };

    enum class atoken_t {
        DATABASE,
        TABLE,
        GET,
        PUT,
        REMOVE
    };
    
    bool operator==(gtoken_t token, const std::string& string) 
    {
        switch (token) {
            case gtoken_t::EXIT: return string == "exit";
            case gtoken_t::CREATE: return string == "create";
            case gtoken_t::DELETE: return string == "delete";
            case gtoken_t::SELECT: return string == "select";
            case gtoken_t::IMPLICIT: return string == "implicit";
        }
    }
    
    bool operator==(atoken_t token, const std::string& string) 
    {
        switch (token) {
            case atoken_t::DATABASE: return string == "database";
            case atoken_t::TABLE: return string == "table";
            case atoken_t::GET: return string == "get";
            case atoken_t::PUT: return string == "put";
            case atoken_t::REMOVE: return string == "remove";
        }
    }
    
    const std::string command_parser::LEGAL_CHARACTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_";
    
    command_parser::command_parser() 
    {
        
    }
    
    prepared_statement command_parser::parse(lsl::span& input_buffer) const 
    {
        prepared_statement statement;
        char* pre_parse_pos = input_buffer.begin();
        std::string string_token; //= input_buffer.next_word
        switch (string_token) {
            case gtoken_t::EXIT:
                statement.set_command(command_t::EXIT);
                return statement;
                
            case gtoken_t::CREATE:
                parse_create_statement(input_buffer, statement);
                return statement;
                
            case gtoken_t::DELETE: 
                parse_delete_statement(input_buffer, statement);
                return statement;
                
            case gtoken_t::SELECT:
                parse_select_statement(input_buffer, statement);
                return statement;
                
            case gtoken_t::IMPLICIT:
                parse_implicit_statement(input_buffer, statement);
                return statement;
                
            default:
                statement.set_error(error_t::PARSING_ERROR, pre_parse_pos);
                return statement;
        }
    }
    
    void command_parser::parse_create_statement(lsl::span& input_buffer, prepared_statement& statement) const
    {
        char* pre_parse_pos = input_buffer.begin();
        std::string string_token; //= input_buffer.next_word
        switch (string_token) {
            case atoken_t::DATABASE:
                statement.set_command(command_t::CREATE_DATABASE);
                parse_arguments(input_buffer, statement, "n");
                return;
                
            case atoken_t::TABLE:
                statement.set_command(command_t::CREATE_TABLE);
                parse_arguments(input_buffer, statement, "n");
                return;
            
            default:
                statement.set_error(error_t::PARSING_ERROR, pre_parse_pos);
                return;
        }
    }
    
    void command_parser::parse_delete_statement(lsl::span& input_buffer, prepared_statement& statement) const
    {
        char* pre_parse_pos = input_buffer.begin();
        std::string string_token; //= input_buffer.next_word
        switch (string_token) {
            case atoken_t::DATABASE:
                statement.set_command(command_t::DELETE_DATABASE);
                parse_arguments(input_buffer, statement, "n");
                return;
                
            case atoken_t::TABLE:
                statement.set_command(command_t::DELETE_TABLE);
                parse_arguments(input_buffer, statement, "n");
                return;
            
            default:
                statement.set_error(error_t::PARSING_ERROR, pre_parse_pos);
                return;
        }
    }
    
    void command_parser::parse_select_statement(lsl::span& input_buffer, prepared_statement& statement) const
    {
        char* pre_parse_pos = input_buffer.begin();
        std::string string_token; //= input_buffer.next_word
        switch (string_token) {
            case atoken_t::DATABASE:
                statement.set_command(command_t::SELECT_DATABASE);
                parse_arguments(input_buffer, statement, "n");
                return;
            
            default:
                statement.set_error(error_t::PARSING_ERROR, pre_parse_pos);
                return;
        }
    }
    void command_parser::parse_implicit_statement(lsl::span& input_buffer, prepared_statement& statement) const
    {
        char* pre_parse_pos = input_buffer.begin();
        std::string string_token; //= input_buffer.next_word
        switch (string_token) {
            case atoken_t::GET:
                statement.set_command(command_t::IMPLICIT_GET);
                parse_arguments(input_buffer, statement, "n v");
                return;
                
            case atoken_t::PUT:
                statement.set_command(command_t::IMPLICIT_PUT);
                parse_arguments(input_buffer, statement, "n v v");
                return;
                
            case atoken_t::REMOVE:
               statement.set_command(command_t::IMPLICIT_REMOVE);
                parse_arguments(input_buffer, statement, "n v");
                return;
                
            default:
                statement.set_error(error_t::PARSING_ERROR, pre_parse_pos);
                return;
        }
    }
    
    void command_parser::parse_arguments(lsl::span& input_buffer, prepared_statement& statement, const std::string& pattern) const
    {
        unsigned int count = std::count(pattern.begin(), pattern.end(), ' ');
        for(unsigned int i = 0; i < count; i++)
        {
            if(!true /*input_buffer has next*/)
            {
                statement.set_error(
                        error_t::ARGUMENT_COUNT_ERROR,
                        utils::string::format("%d arguments are required, only %d are given")
                );
                return;
            }
            
            char argument_char = pattern[2*i];
            std::string next_token; // = input_buffer.next();
            switch (argument_char) {
                case 'n':
                    parse_name_argument(next_token, statement);
                    break;

                case 'v':
                    parse_value_argument(next_token, statement);
                    break;

                default:
                    assert("unrecognized character");
            }
            statement.add_argument(prepared_statement(next_token));
        }
        
        if(!true /*input_buffer has next*/)
        {
            statement.set_error(
                    error_t::ARGUMENT_COUNT_ERROR,
                    utils::string::format("%d arguments are required, and %d are given")
            );
            return;
        }
    }
    
    void command_parser::parse_name_argument(std::string& arg, prepared_statement& statement) const
    {
        unsigned int ill_char_pos = parse_name_argument_characters(arg);
        if(ill_char_pos >= 0)
        {
            statement.set_command(
                    error_t.ILLEGAL_CHARACTER_ERROR,
                    arg.begin() + ill_char_pos
                );
            return;
        }
        
        statement.add_argument(arg);
    }
    
    void command_parser::parse_value_argument(std::string& arg, prepared_statement& statement) const
    {
        if(!strip_outer_parenthesis(arg))
        {
            statement.set_error(
                    error_t.PARENTHESIS_MISMATCH_ERROR,
                    arg
                );
            return;
        }
        un_escape_inner_parenthesis(arg);
        
        statement.add_argument(arg);
    }
    
    unsigned int command_parser::parse_name_argument_characters(const std::string& arg) const
    {
        std::string::size_type ill_char_pos = arg.find_first_not_of(LEGAL_CHARACTERS);
        return (ill_char_pos == arg.npos) ? -1 : ill_char_pos;
    }

    bool command_parser::strip_outer_parenthesis(std::string& arg) const 
    {
        if(*arg.begin() == '"' || *arg.rbegin() == '"')
        {
            if(!(*arg.begin() == '"' && *arg.rbegin() == '"'))
                return false;
            
            arg.pop_back();
            arg.erase(arg.begin());
        }
        
        return true;
    }
    
    void command_parser::un_escape_inner_parenthesis(std::string& arg) const
    {
        arg.replace("\\"", "\"");
    }


} 

