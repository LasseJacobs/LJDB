
#include <fstream>
#include <cstdio>

#include "dbimp.h"
#include "table.h"
#include "utils.h"

#include <iostream>

namespace core {

    DbImp::DbImp(const std::string& root) {
        __root = root;
        if(__root == "/") {
            __root = "";
        }
    }

    Status DbImp::create_table(const std::string& name, const std::string& table_template)
    {
        if(table::exists(__root, name))
            return Status::UNKNOWN_FAILURE;
        
        table table_file(__root, name, table_template);
        if(!table_file.is_open())
            return Status::UNKNOWN_FAILURE;

        return Status::SUCCESS;
    }

    Status DbImp::rename_table(const std::string& old_name, const std::string& new_name)
    {
        table table_file = table::open(__root, old_name);
        table_file.rename(new_name);

        return Status::SUCCESS;
    }

    Status DbImp::remove_table(const std::string& name)
    {
        table table_file = table::open(__root, name);
        table_file.remove();

        return Status::SUCCESS;
    }

    Status DbImp::get(const std::string& table, const std::string& key, std::string& value) const
    {
        return Status::UNSUPPORTED;
    }

    Status DbImp::put(const std::string& table, const std::string& key, const std::string& value)
    {
        return Status::UNSUPPORTED;
    }

    Status DbImp::remove(const std::string& table, const std::string& key)
    {
        return Status::UNSUPPORTED;
    }
}
