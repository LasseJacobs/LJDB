
#include <fstream>
#include <cstdio>

#include "dbimp.h"
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
        std::ofstream table_file;
        std::string filename = __translate_table_name(name);
        if(utils::file::exists(filename))
            return Status::UNKNOWN_FAILURE;

        table_file.open(filename);
        if(!table_file.is_open())
            return Status::UNKNOWN_FAILURE;

        //TODO: validate template <- this should probably happen before creating the file
        table_file << table_template;
        table_file.close();

        return Status::SUCCESS;
    }

    Status DbImp::rename_table(const std::string& old_name, const std::string& new_name)
    {
        std::string old_filename = __translate_table_name(old_name);
        std::string new_filename = __translate_table_name(new_name);
        
        std::rename(old_filename.c_str(), new_filename.c_str());
        return Status::SUCCESS;
    }

    Status DbImp::remove_table(const std::string& name)
    {
        std::string old_filename = __translate_table_name(name);
        std::string new_filename = __translate_table_name(name, __removed_table_log_format);
        if(!utils::file::exists(old_filename))
            return Status::UNKNOWN_FAILURE;
        
        std::rename(old_filename.c_str(), new_filename.c_str());
        return Status::SUCCESS;
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
    
    std::string DbImp::__translate_table_name(const std::string& name)
    {
        return __translate_table_name(name, __table_log_format);
    }
    
    std::string DbImp::__translate_table_name(const std::string& name, const std::string& format)
    {
        std::string filename = utils::string::format(format, name);
        return utils::file::merge_filename(__root, filename);
    }
}
