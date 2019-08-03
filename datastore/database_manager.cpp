
/* 
 * File:   database_manager.cpp
 * Author: lasse
 * 
 * Created on August 3, 2019, 11:24 AM
 */

#include "database_manager.h"
#include "dbinterface.h"
#include "dbimp.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdexcept>


namespace core {
    
    database_manager::database_manager(const std::string& working_dir) 
    {
        struct stat st = {0};
        if (stat(working_dir.c_str(), &st) == -1) {
            throw std::invalid_argument("directory does not exist");
        }
        __pwd = working_dir;
    }

    void database_manager::create_database(const std::string& name)
    {
        std::string db_dir = __pwd + name;
        int result = mkdir(db_dir.c_str(), 0666);
        if(result == -1)
            throw std::runtime_error("failed to create database");
    }
    
    void database_manager::load_database(const std::string& name)
    {
        struct stat st = {0};
        std::string db_dir = __pwd + name;
        if (stat(db_dir.c_str(), &st) == -1) {
            throw std::invalid_argument("directory does not exist");
        }
        
        _current_database = std::make_unique<db_imp>(db_dir, name); 
    }
    
    void database_manager::remove_database(const std::string& name)
    {
        if(_current_database && name == _current_database->database_name())
            _current_database.release();
        
        int result = rename((__pwd + name).c_str(), (__pwd + "r_" + name).c_str());
        if(result != 0) 
        { 
            throw std::runtime_error("failed to remove database");
        }
    }

    const database* database_manager::current_database()
    {
        if(!_current_database)
            throw std::runtime_error("no database loaded");

        return _current_database.get();
    }

}

