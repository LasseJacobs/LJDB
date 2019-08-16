
/* 
 * File:   interpreter.cpp
 * Author: lasse
 * 
 * Created on August 16, 2019, 4:16 PM
 */

#include <map>

#include "interpreter.h"

namespace eval {
    
    interpreter::interpreter(std::ostream& std_out, std::ostream& err_out) :
            __std_out(std_out), __err_out(err_out)
    {

    }
    
    void interpreter::eval(const prepared_statement& statement, LJDBFacade& db, std::map<std::string, std::string>& context) const 
    {
        if(statement.failed())
        {
            print_error(statement.error_type(), statement.cause());
            return;
        }
        
        evalutate_statement(statement, db, context);
    }

    void interpreter::print_error(error_t error_type, const std::string& cause) const 
    {
        __err_out << "could not execute statement, parsing error: " << cause << std::endl;
    }

    void interpreter::evalutate_statement(const prepared_statement& stmt, LJDBFacade& db, std::map<std::string, std::string>& context) const 
    {
        std::string result;
        switch(stmt.command())
        {
            case command_t::EXIT:
                exit(0);
                break;
            case command_t::CREATE_DATABASE:
                db.create_database(stmt.argument(0));
                break;
            case command_t::CREATE_TABLE:
                db.create_table(stmt.argument(1));
                break;
            case command_t::DELETE_DATABASE:
                db.remove_database(stmt.argument(0));
                break;
            case command_t::DELETE_TABLE:
                db.remove_table(stmt.argument(1));
                break;
            case command_t::SELECT_DATABASE:
                db.load_database(stmt.argument(0));
                context.emplace("database", stmt.argument(0));
                break;
            case command_t::IMPLICIT_PUT:
                db.put(stmt.argument(1), stmt.argument(2), stmt.argument(3));
                break;
            case command_t::IMPLICIT_GET:
                db.get(stmt.argument(1), stmt.argument(2), result);
                print_result(result);
                break;
            case command_t::IMPLICIT_REMOVE:
                db.remove(stmt.argument(1), stmt.argument(2));
                break;
        }
    }

    void interpreter::print_result(const std::string& result) const 
    {
        __std_out << "== " << result << std::endl;
    }

}


