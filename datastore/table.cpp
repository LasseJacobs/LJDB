

#include <fstream>
#include <cstdio>
#include <cstring>

#include "table.h"
#include "utils.h"

#include <iostream>


namespace core {
    
    struct row {
        static const size_t BASE_OFFSET = 0;
        static const size_t KEY_SIZE = 32;
        char key_buffer[KEY_SIZE];
        static const size_t VALUE_SIZE = 144;
        char value_buffer[VALUE_SIZE];
    };
    
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
        
        __table_file = std::make_shared<std::fstream>(filename, std::fstream::in | std::fstream::out | std::fstream::binary | std::fstream::trunc);
        if(!__table_file->is_open()) {
            return;
        }
        
        //TODO: validate template <- this should probably happen before creating the file
        //__table_file->write(table_template.c_str(), table_template.size());
    }
    
    table::table(const table& other)
    {
        __root = other.__root;
        __table_name = other.__table_name;
        
        __table_file = other.__table_file;
    }

    
    table::~table() 
    {
        if(__table_file->is_open() && __table_file.use_count() == 0)
            __table_file->close();
    }
    
    Status table::remove()
    {
        __table_file->close();
        
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
    
    Status table::put(const std::string& key, const std::string& value)
    {
        row row_record;
        std::memcpy(&row_record.key_buffer, key.c_str(), key.size() + 1);
        std::memcpy(&row_record.value_buffer, value.c_str(), value.size() + 1);
                
        __table_file->write((char*)&row_record, sizeof(row));
        if(__table_file->fail())
            return Status::UNKNOWN_FAILURE;
        
        return Status::SUCCESS;
    }
    
    Status table::get(const std::string& key, std::string& value) const 
    {
        row row_record;
        __table_file->seekg(0, std::ios::end);
        long cursor = (long)__table_file->tellg() - (long)sizeof(row); 
        
        while(cursor < 0) 
        {
            __table_file->seekg(cursor);
            __table_file->read((char*)&row_record, sizeof(row));
            if(__table_file->fail())
                return Status::UNKNOWN_FAILURE;
            
            std::string current_key = row_record.key_buffer;
            if(current_key == key)
            {
                value = std::string(row_record.value_buffer);
                break;
            }
            
            cursor -= sizeof(row);
        }
      
        return Status::SUCCESS;
    }

    table::table(const std::string& root, const std::string& name)
    {
        __root = root;
        __table_name = name;
        
        std::string filename = __translate_table_name(name);
        if(!utils::file::exists(filename))
            return;
        
        __table_file = std::make_shared<std::fstream>();
        __table_file->open(filename, std::fstream::in | std::fstream::out | std::fstream::app | std::fstream::binary);
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
