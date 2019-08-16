
/* 
 * File:   command_parser.cpp
 * Author: lasse
 * 
 * Created on August 10, 2019, 10:23 AM
 */

#include "command_parser.h"
#include "utils.h"
#include <assert.h>
#include <map>     



namespace eval {
    
    const std::string command_parser::LEGAL_CHARACTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_";
    
    command_parser::command_parser() 
    {
        
    }
    
    prepared_statement command_parser::parse(std::queue<token_t> tokens, const std::map<std::string, std::string>& context) const 
    {
        prepared_statement statement;
        token_t token = tokens.front();
        tokens.pop();
        switch (token.token_type) {
            case token_type_t::EXIT:
                statement.set_command(command_t::EXIT);
                break;
                
            case token_type_t::CREATE:
                parse_create_statement(tokens, context, statement);
                break;
                
            case token_type_t::DELETE: 
                parse_delete_statement(tokens, context, statement);
                break;
                
            case token_type_t::SELECT:
                parse_select_statement(tokens, context, statement);
                break;
                
            case token_type_t::GET:
                statement.set_command(command_t::IMPLICIT_GET);
                parse_arguments(tokens, context, statement, "c n v");
                break;
                
            case token_type_t::PUT:
                statement.set_command(command_t::IMPLICIT_PUT);
                parse_arguments(tokens, context, statement, "c n v v");
                break;
                
            case token_type_t::REMOVE:
                statement.set_command(command_t::IMPLICIT_REMOVE);
                parse_arguments(tokens, context, statement, "c n v");
                break;
                
            default:
                if(token.token_type == token_type_t::IDENTIFIER_ARGUMENT)
                    statement.set_error(
                            error_t::PARSING_ERROR, 
                            utils::string::format("unknown identifier: %s", token.token_value));
                else if(token.token_type == token_type_t::STRING_ARGUMENT)
                    statement.set_error(
                            error_t::PARSING_ERROR, 
                            utils::string::format("expecting keyword, found %s", token.token_value));
                else
                    statement.set_error(
                            error_t::PARSING_ERROR, 
                            utils::string::format("unexpected keyword: %s", token.token_value));

        }

        return statement;
    }
    
    void command_parser::parse_create_statement(std::queue<token_t>& tokens, const std::map<std::string, std::string>& context, prepared_statement& statement) const
    {
        token_t token = tokens.front();
        tokens.pop();
        switch (token.token_type) {
            case token_type_t::DATABASE:
                statement.set_command(command_t::CREATE_DATABASE);
                parse_arguments(tokens, context, statement, "n");
                return;
                
            case token_type_t::TABLE:
                statement.set_command(command_t::CREATE_TABLE);
                parse_arguments(tokens, context, statement, "c n");
                return;
            
            default:
                statement.set_error(
                        error_t::PARSING_ERROR, 
                        utils::string::format("unexpected keyword %s, was expecting specifier", token.token_value)
                );
                return;
        }
    }
    
    void command_parser::parse_delete_statement(std::queue<token_t>& tokens, const std::map<std::string, std::string>& context, prepared_statement& statement) const
    {
        token_t token = tokens.front();
        tokens.pop();
        switch (token.token_type) {
            case token_type_t::DATABASE:
                statement.set_command(command_t::DELETE_DATABASE);
                parse_arguments(tokens, context, statement, "n");
                return;
                
            case token_type_t::TABLE:
                statement.set_command(command_t::DELETE_TABLE);
                parse_arguments(tokens, context, statement, "c n");
                return;
            
            default:
                statement.set_error(
                        error_t::PARSING_ERROR, 
                        utils::string::format("unexpected keyword %s, was expecting specifier", token.token_value)
                );
                return;
        }
    }
    
    void command_parser::parse_select_statement(std::queue<token_t>& tokens, const std::map<std::string, std::string>& context, prepared_statement& statement) const
    {
        token_t token = tokens.front();
        tokens.pop();
        switch (token.token_type) {
            case token_type_t::DATABASE:
                statement.set_command(command_t::SELECT_DATABASE);
                parse_arguments(tokens, context, statement, "n");
                return;
            
            default:
                statement.set_error(
                        error_t::PARSING_ERROR, 
                        utils::string::format("unexpected keyword %s, was expecting specifier", token.token_value)
                );
                return;
        }
    }
    
    void command_parser::parse_arguments(std::queue<token_t>& tokens, const std::map<std::string, std::string>& context, prepared_statement& statement, const std::string& pattern) const
    {
        //TODO: this needs to get cleaned up
        unsigned int count = std::count(pattern.begin(), pattern.end(), ' ') + 1;
        for(unsigned int i = 0; i < count; i++)
        {
            if(tokens.empty())
            {
                statement.set_error(
                        error_t::PARSING_ERROR,
                        utils::string::format("%d arguments are required, only %d are given", count, i)
                );
                return;
            }
            
            char argument_char = pattern[2*i];
            token_t next_token;
            switch (argument_char) {
                case 'n':
                    next_token = tokens.front();
                    tokens.pop();
                    parse_name_argument(next_token, statement);
                    break;

                case 'v':
                    next_token = tokens.front();
                    tokens.pop();
                    parse_value_argument(next_token, statement);
                    break;
                    
                case 'c':
                    get_context(context, statement);
                    break;

                default:
                    assert("unrecognized character");
            }
        }
        
        if(!tokens.empty())
        {
            statement.set_error(
                    error_t::PARSING_ERROR,
                    utils::string::format("only %d arguments expected", count)
            );
            return;
        }
    }
    
    void command_parser::parse_name_argument(const token_t& arg, prepared_statement& statement) const
    {
        int ill_char_pos = find_illegal_characters(arg.token_value);
        if(ill_char_pos >= 0)
        {
            statement.set_error(
                    error_t::PARSING_ERROR,
                    utils::string::format("illegal character at %s:%d", arg.token_value, ill_char_pos)
                );
            return;
        }
        
        statement.add_argument(arg.token_value);
    }
    
    void command_parser::parse_value_argument(const token_t& arg, prepared_statement& statement) const
    {
        std::string token_value = arg.token_value;
        if(!strip_outer_parenthesis(token_value))
        {
            statement.set_error(
                    error_t::PARSING_ERROR,
                    utils::string::format("could not properly parse string, %s", token_value)
                );
            return;
        }
        un_escape_inner_parenthesis(token_value);
        
        statement.add_argument(token_value);
    }
    
    void command_parser::get_context(const std::map<std::string, std::string>& context, prepared_statement& statement) const
    {
        auto result = context.find("database");
        if(result == context.end())
        {
            statement.set_error(
                        error_t::PARSING_ERROR, 
                        "required active context (database), but none was set. Use 'select <database>' before issuing context specific commands."
                );
        }
        
        statement.add_argument(result->second);
    }
    
    int command_parser::find_illegal_characters(const std::string& arg) const
    {
        std::string::size_type ill_char_pos = arg.find_first_not_of(LEGAL_CHARACTERS);
        return (ill_char_pos == arg.npos) ? -1 : (int)ill_char_pos;
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
        //arg.replace("\\"", "\"");
    }


} 
