
/* 
 * File:   interpreter.h
 * Author: lasse
 *
 * Created on August 16, 2019, 4:16 PM
 */

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "prepared_statement.h"
#include "dbfacade.h"

#include <ostream>


namespace eval {

class interpreter {
public:
    interpreter(std::ostream& std_out, std::ostream& err_out);
    
    void eval(const prepared_statement& statement, LJDBFacade& db, std::map<std::string, std::string>& context) const;
    
private:
    std::ostream& __std_out;
    std::ostream& __err_out;
    
    void print_error(error_t error_type, const std::string& cause) const;
    void evalutate_statement(const prepared_statement& statement, LJDBFacade& db, std::map<std::string, std::string>& context) const;
    void print_result(const std::string& result) const;

};

}

#endif /* INTERPRETER_H */

