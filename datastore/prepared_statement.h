
/* 
 * File:   prepared_statement.h
 * Author: lasse
 *
 * Created on August 10, 2019, 11:04 AM
 */

#ifndef PREPARED_STATEMENT_H
#define PREPARED_STATEMENT_H

#include <vector>
#include <string>

namespace interpreting {

enum class statement_t {
    META,
    CONCRETE,
    FAILURE
};

enum class command_t {
    EXIT,
    CREATE_DATABASE,
    CREATE_TABLE,
    DELETE_DATABASE,
    DELETE_TABLE,
    SELECT_DATABASE,
    IMPLICIT_PUT,
    IMPLICIT_GET,
    IMPLICIT_REMOVE
};

enum class error_t {
    PARSING_ERROR,
    ILLEGAL_CHARACTER_ERROR,
    ARGUMENT_COUNT_ERROR,
    PARENTHESIS_MISMATCH_ERROR
};
    
class prepared_statement {
public:
    prepared_statement();
    
    void set_command(command_t command);
    void set_error(error_t error, const std::string& cause);
    
    void add_argument(const std::string& arg);
    
    statement_t statement_type() const;
    command_t command() const;
    
    error_t error_type() const;
    const std::string& cause() const;
    
private:
    statement_t __type;
    
    command_t __command;
    error_t __error;
    
    std::vector<std::string> __arguments;
    std::string __error_message;
    
    bool is_meta(command_t cmd) const;
};

}

#endif /* PREPARED_STATEMENT_H */

