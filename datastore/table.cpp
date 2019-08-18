

#include <fstream>
#include <cstdio>
#include <cstring>

#include "table.h"
#include "utils.h"
#include "encoder.h"
#include "decoder_iterator.h"
#include "database_exceptions.h"

#include <iostream>


namespace core {
        
    const std::string table::TABLE_LOG_FORMAT = "v_%s_log.table";
    const std::string table::REMOVED_TABLE_LOG_FORMAT = "r_%s_log.table";

    bool table::exists(const std::string& root, const std::string& name)
    {
        std::string filename = utils::string::format(TABLE_LOG_FORMAT, name);
        filename = utils::file::merge_filename(root, filename);
        
        return utils::file::exists(filename);
    }
    
    table table::open(const std::string& root, const std::string& name)
    {
        std::string filename = __translate_table_name(root, name);
        if(!utils::file::exists(filename))
            throw unknown_argument_exception(name);
        
        table new_table;
        new_table.__root = root;
        new_table.__table_name = name;
       
        new_table.__main_log.open(filename);
        
        log::iterator itr = new_table.__main_log.begin();
        while(itr.has_next())
        {
            size_t pre_read_pos = itr.current_position();
            std::pair<std::string, std::string> row;
            try {
                row = itr.next();
            }
            catch(const std::exception& e) {
                throw parse_failure_exception(filename, pre_read_pos);
            }   
            
            if(row.second == log::TOMBSTONE_TOKEN)
            {
                new_table.__key_index.erase(row.first);
            }
            else
            {
                new_table.__key_index.emplace(row.first, pre_read_pos);
            }
        }
        
        return new_table;
    }

    table::table(const std::string& root, const std::string& name)
    {
        __root = root;
        __table_name = name;
        
        std::string filename = __translate_table_name(root, name);
        if(utils::file::exists(filename))
            throw already_exists_exception(name);
        
        __main_log = log(filename);
    }
    
    void table::delete_table()
    {
        __main_log.delete_log();
        __key_index.clear();
    }
    
    void table::put(const std::string& key, const std::string& value)
    {
        uint32_t insert_index = __main_log.put(key, value);         
        __key_index.emplace(key, insert_index);

    }
    
    void table::get(const std::string& key, std::string& value) const 
    {
        auto file_offset_iterator = __key_index.find(key);
        if (file_offset_iterator == __key_index.end())
            throw unknown_argument_exception(key);
        
        __main_log.get(file_offset_iterator->second, value);        
    }
    
    void table::remove(const std::string& key)
    {
        __main_log.remove(key);
        __key_index.erase(key);
    }

    table::table()
    {
        
    }
    
    std::string table::__translate_table_name(const std::string& root, const std::string& name, const std::string& format)
    {
        std::string filename = utils::string::format(format, name);
        return utils::file::merge_filename(root, filename);
    }

}
