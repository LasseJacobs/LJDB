
/* 
 * File:   dbfacade.h
 * Author: lasse
 *
 * Created on August 3, 2019, 11:18 AM
 */

#ifndef DBFACADE_H
#define DBFACADE_H

#include <string>
#include "database_manager.h"
#include "database.h"


class LJDBFacade {
public:
    LJDBFacade(const std::string& path);

    void create_database(const std::string& name);
    void load_database(const std::string& name);
    void remove_database(const std::string& name);

    void create_table(const std::string& name);
    void remove_table(const std::string& name);

    void get(const std::string& table, const std::string& key, std::string& value) const;
    void put(const std::string& table, const std::string& key, const std::string& value);
    void remove(const std::string& table, const std::string& key);

private:
    core::database_manager __dbm;
    core::database* __database; 
        

};

#endif /* DBFACADE_H */

