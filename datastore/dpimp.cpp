
#include <fstream>
#include <cstdio>

#include "dbimp.h"
#include "table.h"
#include "utils.h"
#include "database_exceptions.h"

#include <iostream>

namespace core {

    db_imp::db_imp(const std::string& root, const std::string& name) 
    {
        __root = root;
        __name = name;
        if(__root == "/") 
        {
            __root = "";
        }
    }
    
    const std::string& db_imp::database_name() const 
    {
        return __name;
    }

    void db_imp::create_table(const std::string& name)
    {
        if(table::exists(__root, name))
            throw already_exists_exception(name);
        
        table table_file(__root, name);
        __open_tables.emplace(name, table_file);
    }

    void db_imp::remove_table(const std::string& name)
    {
        if(__open_tables.count(name) == 0)
            __open_tables.emplace(name, table::open(__root, name));
        
        table& table_file = __open_tables.find(name)->second;
        table_file.delete_table();
        __open_tables.erase(name);
    }

    void db_imp::get(const std::string& table_name, const std::string& key, std::string& value) const
    {
        if(__open_tables.count(table_name) == 0)
            __open_tables.emplace(table_name, table::open(__root, table_name));
        
        table& table_file = __open_tables.find(table_name)->second;
        table_file.get(key, value);
    }

    void db_imp::put(const std::string& table_name, const std::string& key, const std::string& value)
    {
        if(__open_tables.count(table_name) == 0)
            __open_tables.emplace(table_name, table::open(__root, table_name));
        
        table& table_file = __open_tables.find(table_name)->second;
        table_file.put(key, value);
    }

    void db_imp::remove(const std::string& table_name, const std::string& key)
    {
        if(__open_tables.count(table_name) == 0)
            __open_tables.emplace(table_name, table::open(__root, table_name));
        
        table& table_file = __open_tables.find(table_name)->second;
        table_file.remove(key);
    }
}
