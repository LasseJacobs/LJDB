
/* 
 * File:   database_manager.cpp
 * Author: lasse
 * 
 * Created on August 3, 2019, 11:24 AM
 */

#include "database_manager.h"
#include "database.h"
#include "dbimp.h"
#include "utils.h"
#include "database_exceptions.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdexcept>


namespace core {
    
    database_manager::database_manager(const std::string& working_dir) 
    {
        if (!utils::file::exists_dir(working_dir)) {
            throw unknown_argument_exception(working_dir);
        }
        __pwd = working_dir;
    }

    void database_manager::create_database(const std::string& name)
    {
        std::string db_dir = __pwd + name;
        if (utils::file::exists_dir(db_dir)) {
            throw already_exists_exception(db_dir);
        }
        
        int result = mkdir(db_dir.c_str(), 0777);
        if(result == -1)
            throw io_failure_exception("failed to create database");
    }
    
    database* database_manager::load_database(const std::string& name)
    {
        std::string db_dir = __pwd + name;
        if (!utils::file::exists_dir(db_dir)) {
            throw unknown_argument_exception(name);
        }
        
        _current_database = std::make_unique<db_imp>(db_dir, name);
        return _current_database.get();
    }
    
    void database_manager::remove_database(const std::string& name)
    {
        if(_current_database && name == _current_database->database_name())
            _current_database.release();
        
        int result = rename((__pwd + name).c_str(), (__pwd + "r_" + name).c_str());
        if(result != 0) 
        { 
            throw io_failure_exception("failed to remove database");
        }
    }

    database* database_manager::current_database() const
    {
        if(!_current_database)
            throw std::runtime_error("no database loaded");

        return _current_database.get();
    }

}

