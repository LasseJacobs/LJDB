
/* 
 * File:   database_manager.h
 * Author: lasse
 *
 * Created on August 3, 2019, 11:24 AM
 */

#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <string>
#include <memory>

#include "database.h"

namespace core {

class database_manager {
public:
    database_manager(const std::string& working_dir);
    
    void create_database(const std::string& name);
    void load_database(const std::string& name);
    void remove_database(const std::string& name);
    
    const database* current_database();
    
private:
    std::string __pwd;
    std::unique_ptr<database> _current_database;

};

}

#endif /* DATABASE_MANAGER_H */

