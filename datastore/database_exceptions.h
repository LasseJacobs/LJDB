
/* 
 * File:   database_exception.h
 * Author: lasse
 *
 * Created on August 6, 2019, 7:32 PM
 */

#ifndef DATABASE_EXCEPTION_H
#define DATABASE_EXCEPTION_H

#include <exception>
#include <string>
#include "utils.h"

#define DEFAULT_CAUSE "an undocumented exception occured"

namespace core {

class database_exception : public std::exception {
public:
    database_exception() : __cause(DEFAULT_CAUSE)
    {
        
    }
    
    database_exception(const std::string& cause) : __cause(cause)
    {
        
    }
    
    const char* what() const noexcept override 
    {
        return __cause.c_str();
    }
    
private:
    const std::string __cause;
};

class expected_exception : public database_exception {
public:
    expected_exception()
    {
        
    }
    
    expected_exception(const std::string& cause) : database_exception(cause)
    {
        
    }
};


class unexpected_exception : public database_exception {
public:
    unexpected_exception()
    {
        
    }
    
    unexpected_exception(const std::string& cause) : database_exception(cause)
    {
        
    }
};


class already_exists_exception : public expected_exception {
public:
    
    already_exists_exception(const std::string& item) 
            : expected_exception(utils::string::format("item '%s' already exists and cannot be created again", item))
    {
        
    }
};


class unknown_argument_exception : public expected_exception {
public:
    unknown_argument_exception(const std::string& argument) 
        : expected_exception(utils::string::format("unknown argument: '%s'", argument))
    {
        
    }
};


class io_failure_exception : public unexpected_exception {
public:
    io_failure_exception(const std::string& cause = "an IO operation failed") 
            : unexpected_exception(cause)
    {
        
    }
            
    io_failure_exception(const std::exception& e) 
            : unexpected_exception(utils::string::format("an IO operation failed because '%s'", e.what()))
    {
        
    }
            
    io_failure_exception(const std::string& cause, const std::string& io_failure) 
            : unexpected_exception(utils::string::format("%s caused by '%s'", cause, io_failure))
    {
        
    }
};

class parse_failure_exception : public io_failure_exception {
public:
    parse_failure_exception(const std::string& faulty_string) 
        : io_failure_exception(utils::string::format("failed to parse: '%s'", faulty_string))
    {
        
    }
    
    parse_failure_exception(const std::string& file, unsigned long line) 
        : io_failure_exception(utils::string::format("failed to parse %s:%d", file, line))
    {
        
    }
};

}

#endif /* DATABASE_EXCEPTION_H */

