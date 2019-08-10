

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
    const std::string table::TOMBSTONE_TOKEN = "\0\0\0\0";

    bool table::exists(const std::string& root, const std::string& name)
    {
        std::string filename = utils::string::format(TABLE_LOG_FORMAT, name);
        filename = utils::file::merge_filename(root, filename);
        
        return utils::file::exists(filename);
    }
    
    table table::open(const std::string& root, const std::string& name)
    {
        table new_table;
        new_table.__root = root;
        new_table.__table_name = name;
        std::string filename = __translate_table_name(root, name);
        if(!utils::file::exists(filename))
            throw unknown_argument_exception(name);
        
        new_table.__table_file = std::make_shared<std::fstream>();
        new_table.__table_file->open(filename, std::fstream::in | std::fstream::out | std::fstream::app | std::fstream::binary);
        if(!new_table.__table_file->is_open())
            throw io_failure_exception("could not open table structure");
        
        data::decoder_iterator<std::string, std::string> itr(new_table.__table_file.get());
        while(itr.has_next())
        {
            size_t pre_read_pos = new_table.__table_file->tellg();
            
            std::pair<std::string, std::string> row;
            try {
                row = itr.next();
            }
            catch(const std::exception& e) {
                new_table.__table_file->close();
                throw parse_failure_exception(filename, pre_read_pos);
            }   
            
            if(row.second == TOMBSTONE_TOKEN)
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
            return;
        
        __table_file = std::make_shared<std::fstream>(filename, std::fstream::in | std::fstream::out | std::fstream::binary | std::fstream::trunc);
        if(!__table_file->is_open()) {
            return;
        }
    }

    table::~table() 
    {
        if(__table_file->is_open() && __table_file.use_count() == 1)
            __table_file->close();
    }
    
    void table::delete_table()
    {
        __table_file->close();
        
        std::string old_filename = __translate_table_name(__root, __table_name);
        std::string new_filename = __translate_table_name(__root, __table_name, REMOVED_TABLE_LOG_FORMAT);
        if(!utils::file::exists(old_filename))
            throw unknown_argument_exception(old_filename);
        
        std::rename(old_filename.c_str(), new_filename.c_str());
    }
    
    bool table::is_open() const
    {
        return __table_file->is_open();
    }
    
    void table::put(const std::string& key, const std::string& value)
    {     
        data::encoder data_encoder;
        data::blob encoded_pair = data_encoder.encode(std::make_pair(key, value));
        
        size_t pre_write_index = __table_file->tellp();
        __table_file->write(encoded_pair.data, encoded_pair.length);
        if(__table_file->fail())
            throw io_failure_exception();
        
        __key_index.emplace(key, pre_write_index);

    }
    
    void table::get(const std::string& key, std::string& value) const 
    {
        auto file_offset_iterator = __key_index.find(key);
        if (file_offset_iterator == __key_index.end())
            throw unknown_argument_exception(key);
        
        try {
            data::decoder_iterator<std::string, std::string> itr(__table_file.get(), file_offset_iterator->second);
            value = itr.next().second;
        } 
        catch(const std::exception& e) {
            throw io_failure_exception();
        }
                
    }
    
    void table::remove(const std::string& key)
    {
        put(key, TOMBSTONE_TOKEN);
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
