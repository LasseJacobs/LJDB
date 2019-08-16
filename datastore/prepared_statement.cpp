
#include <stdexcept>

#include "prepared_statement.h"
#include "utils.h"

namespace eval {
    
    prepared_statement::prepared_statement()
    {
        __error = error_t::NONE;
    }
    
    void prepared_statement::set_command(command_t command)
    {
        if(__error != error_t::NONE)
            return;
        
        __command = command;
    }
    
    void prepared_statement::set_error(error_t error, const std::string& cause)
    {
        if(__error != error_t::NONE) //todo potentially keep list of all errors
            return;
        
        __error = error;
        __error_message = cause;
    }
    
    void prepared_statement::add_argument(const std::string& arg)
    {
        __arguments.push_back(arg);
    }
    
    command_t prepared_statement::command() const
    {
        return __command;
    }
    
    const std::string& prepared_statement::argument(uint32_t index) const
    {
        if(__arguments.size() <= index)
            throw std::range_error(utils::string::format("statement only has %d arguments", __arguments.size()));
        
        return __arguments[index];
    }

    bool prepared_statement::failed() const 
    {
        return __error != error_t::NONE;
    }
    
    error_t prepared_statement::error_type() const
    {
        return __error;
    }
    
    const std::string& prepared_statement::cause() const
    {
        return __error_message;
    }
    
}