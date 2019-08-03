

#include <fstream>
#include <cstdio>
#include <cstring>

#include "table.h"
#include "utils.h"
#include "encoder.h"
#include "decoder_iterator.h"

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

    table::~table() 
    {
        if(__table_file->is_open() && __table_file.use_count() == 1)
            __table_file->close();
    }
    
    Status table::remove()
    {
        __table_file->close();
        
        std::string old_filename = __translate_table_name(__table_name);
        std::string new_filename = __translate_table_name(__table_name, REMOVED_TABLE_LOG_FORMAT);
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
        /*
        row row_record;
        std::memcpy(&row_record.key_buffer, key.c_str(), key.size() + 1);
        std::memcpy(&row_record.value_buffer, value.c_str(), value.size() + 1);
        */
        
        data::encoder data_encoder;
        data::blob encoded_pair = data_encoder.encode(std::make_pair(key, value));
        
        size_t pre_write_index = __table_file->tellp();
        __table_file->write(encoded_pair.data, encoded_pair.length);
        if(__table_file->fail())
            return Status::UNKNOWN_FAILURE;
        
        __key_index.emplace(key, pre_write_index);

        return Status::SUCCESS;
    }
    
    Status table::get(const std::string& key, std::string& value) const 
    {
        //row row_record;
        /*
        __table_file->seekg(0, std::ios::end);
        long cursor = (long)__table_file->tellg() - (long)sizeof(row); 
        
        while(cursor >= 0) 
        {
            __table_file->seekg(cursor);
            __table_file->read((char*)&row_record, sizeof(row));
            if(__table_file->fail())
                return Status::UNKNOWN_FAILURE;
            
            if(row_record.value_buffer[0] == '\0')
            {
                return Status::CLIENT_ERROR;
            }
            else if(std::strcmp(row_record.key_buffer, key.c_str()) == 0)
            {
                value = std::string(row_record.value_buffer);
                return Status::SUCCESS;
            }
            
            cursor -= sizeof(row);
        }
        */
        auto file_offset_iterator = __key_index.find(key);
        if (file_offset_iterator == __key_index.end())
            return Status::CLIENT_ERROR;
        
        try {
            data::decoder_iterator<std::string, std::string> itr(__table_file.get(), file_offset_iterator->second);
            value = itr.next().second;
        } 
        catch(const std::exception& e) {
            return Status::UNKNOWN_FAILURE;
        }
                
        return Status::SUCCESS;
    }
    
    Status table::remove(const std::string& key)
    {
        put(key, TOMBSTONE_TOKEN);
       
        __key_index.erase(key);
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
        
        /*
        row row_record;
        __table_file->seekg(0, std::ios::beg);
        while(!__table_file->eof())
        {
            size_t pre_read_pos = __table_file->tellg();
            __table_file->read((char*)&row_record, sizeof(row));
            if(__table_file->fail()) {
                __table_file->close();
                return;
            }
            
            if(row_record.value_buffer[0] == '\0')
            {
                __key_index.erase(std::string(row_record.key_buffer));
            }
            else
            {
                __key_index.emplace(std::string(row_record.key_buffer), pre_read_pos);
            }
        }
        */
        
        data::decoder_iterator<std::string, std::string> itr(__table_file.get());
        while(itr.has_next())
        {
            size_t pre_read_pos = __table_file->tellg();
            
            std::pair<std::string, std::string> row;
            try {
                row = itr.next();
            }
            catch(const std::exception& e) {
                __table_file->close();
                return;
            }   
            
            if(row.second == TOMBSTONE_TOKEN)
            {
                __key_index.erase(row.first);
            }
            else
            {
                __key_index.emplace(row.first, pre_read_pos);
            }
        }
        
    }

    std::string table::__translate_table_name(const std::string& name) const
    {
        return __translate_table_name(name, TABLE_LOG_FORMAT);
    }
    
    std::string table::__translate_table_name(const std::string& name, const std::string& format) const
    {
        std::string filename = utils::string::format(format, name);
        return utils::file::merge_filename(__root, filename);
    }

}
