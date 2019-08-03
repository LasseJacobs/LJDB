
#include <fstream>
#include <cstdio>

#include "dbimp.h"
#include "table.h"
#include "utils.h"

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

    Status db_imp::create_table(const std::string& name)
    {
        if(table::exists(__root, name))
            return Status::UNKNOWN_FAILURE;
        
        table table_file(__root, name);
        if(!table_file.is_open())
            return Status::UNKNOWN_FAILURE;
        
        __open_tables.emplace(name, table_file);
        return Status::SUCCESS;
    }

    Status db_imp::remove_table(const std::string& name)
    {
        if(__open_tables.count(name) == 0)
            __open_tables.emplace(name, table::open(__root, name));
        
        table& table_file = __open_tables.find(name)->second;
        table_file.remove();
        __open_tables.erase(name);

        return Status::SUCCESS;
    }

    Status db_imp::get(const std::string& table_name, const std::string& key, std::string& value) const
    {
        if(__open_tables.count(table_name) == 0)
            __open_tables.emplace(table_name, table::open(__root, table_name));
        
        table& table_file = __open_tables.find(table_name)->second;
        return table_file.get(key, value);
    }

    Status db_imp::put(const std::string& table_name, const std::string& key, const std::string& value)
    {
        if(__open_tables.count(table_name) == 0)
            __open_tables.emplace(table_name, table::open(__root, table_name));
        
        table& table_file = __open_tables.find(table_name)->second;
        return table_file.put(key, value);
    }

    Status db_imp::remove(const std::string& table_name, const std::string& key)
    {
        if(__open_tables.count(table_name) == 0)
            __open_tables.emplace(table_name, table::open(__root, table_name));
        
        table& table_file = __open_tables.find(table_name)->second;
        return table_file.remove(key);
    }
}
