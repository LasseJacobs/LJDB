

#include <fstream>
#include <cstdio>

#include "table.h"
#include "utils.h"

#include <iostream>


namespace core {
    
    const std::string table::__table_log_format = "v_%s_log.table";
    const std::string table::__removed_table_log_format = "r_%s_log.table";

    bool table::exists(const std::string& root, const std::string& name)
    {
        std::string filename = utils::string::format(__table_log_format, name);
        filename = utils::file::merge_filename(root, filename);
        
        return utils::file::exists(filename);
    }
    
    table table::open(const std::string& root, const std::string& name)
    {
        return table(root, name);
    }

    table::table(const std::string& root, const std::string& name, const std::string& table_template)
    {
        __root = root;
        __table_name = name;
        
        std::string filename = __translate_table_name(name);
        if(utils::file::exists(filename))
            return;
        
        __table_file = std::make_shared<std::ofstream>(filename);
        if(!__table_file->is_open()) {
            return;
        }
        
        //TODO: validate template <- this should probably happen before creating the file
        __table_file->write(table_template.c_str(), table_template.size());
    }
    
    table::table(const table& other)
    {
        __root = other.__root;
        __table_name = other.__table_name;
        
        __key_size = other.__key_size;
        __value_size = other.__value_size;
        
        __table_file = other.__table_file;
    }

    
    table::~table() 
    {
        //__table_file->close();
    }

    Status table::rename(const std::string& new_name)
    {        
        std::string old_filename = __translate_table_name(__table_name);
        std::string new_filename = __translate_table_name(new_name);
        
        std::rename(old_filename.c_str(), new_filename.c_str());
        return Status::SUCCESS;
    }
    
    Status table::remove()
    {
        std::string old_filename = __translate_table_name(__table_name);
        std::string new_filename = __translate_table_name(__table_name, __removed_table_log_format);
        if(!utils::file::exists(old_filename))
            return Status::UNKNOWN_FAILURE;
        
        std::rename(old_filename.c_str(), new_filename.c_str());
        return Status::SUCCESS;
    }
    
    bool table::is_open() 
    {
        return __table_file->is_open();
    }
    
    table::table(const std::string& root, const std::string& name)
    {
        __root = root;
        __table_name = name;
        
        std::string filename = __translate_table_name(name);
        if(!utils::file::exists(filename))
            return;
        
        __table_file = std::make_shared<std::ofstream>();
        __table_file->open(filename, std::ofstream::app);
    }

    std::string table::__translate_table_name(const std::string& name) const
    {
        return __translate_table_name(name, __table_log_format);
    }
    
    std::string table::__translate_table_name(const std::string& name, const std::string& format) const
    {
        std::string filename = utils::string::format(format, name);
        return utils::file::merge_filename(__root, filename);
    }

}
