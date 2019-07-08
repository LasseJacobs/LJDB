
#include <fstream>
#include "dbimp.h"
#include "utils.h"

#include <iostream>

namespace core {

    DbImp::DbImp(const std::string& root) {
        _root = root;
        if(_root == "/") {
            _root = "";
        }
    }


    Status DbImp::create_table(const std::string& name, const std::string& table_template)
    {
        std::ofstream table_file;
        std::string filename = utils::string::format(_table_log_format, name);
        filename = utils::file::merge_filename(_root, filename);
        if(utils::file::exists(filename))
            return Status::UNKNOWN_FAILURE;

        table_file.open(filename);
        if(!table_file.is_open())
            return Status::UNKNOWN_FAILURE;
        //TODO: validate template
        table_file << table_template;
        table_file.close();

        return Status::SUCCESS;
    }

    Status DbImp::rename_table(const std::string& name)
    {
        return Status::UNSUPPORTED;
    }

    Status DbImp::remove_table(const std::string& name)
    {
        return Status::UNSUPPORTED;
    }

    Status DbImp::get(const std::string& table, const std::string& key, std::string& value)
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
