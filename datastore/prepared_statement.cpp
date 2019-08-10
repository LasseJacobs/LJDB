
#include "prepared_statement.h"

namespace interpreting {
    
    prepared_statement::prepared_statement()
    {
        __type = statement_t::META;
    }
    
    void prepared_statement::set_command(command_t command)
    {
        if(__type == statement_t::FAILURE)
            return;
        
        __type = is_meta() ? statement_t::META : statement_t::CONCRETE;
        __command = command;
    }
    
    void prepared_statement::set_error(error_t error, const std::string& cause)
    {
        __type = statement_t::FAILURE;
        __error = error;
        __error_message = cause;
    }
    
    void prepared_statement::add_argument(const std::string& arg)
    {
        __arguments.push_back(arg);
    }
    
    statement_t prepared_statement::statement_type() const
    {
        return __type;
    }
    
    command_t prepared_statement::command() const
    {
        return __command;
    }
    
    error_t prepared_statement::error_type() const
    {
        return __error;
    }
    
    const std::string& prepared_statement::cause() const
    {
        return __error_message;
    }
    
    
    bool prepared_statement::is_meta(command_t cmd) const
    {
        return cmd == command_t::EXIT;
    }
    
}