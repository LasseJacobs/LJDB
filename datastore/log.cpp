
/* 
 * File:   log.cpp
 * Author: lasse
 * 
 * Created on August 17, 2019, 10:58 AM
 */

#include "log.h"

#include <fstream>
#include <cstdio>
#include <cstring>

#include "utils.h"
#include "encoder.h"
#include "decoder_iterator.h"
#include "database_exceptions.h"

#include <iostream>


namespace core {
        
    const std::string log::TOMBSTONE_TOKEN = "\0\0\0\0";
    const std::ios_base::openmode log::NEW_LOG_MODE = std::ios_base::in | std::ios_base::out | std::ios_base::app | std::ios_base::binary | std::ios_base::trunc;
    const std::ios_base::openmode log::OPEN_LOG_MODE = std::ios_base::in | std::ios_base::out | std::ios_base::app | std::ios_base::binary;
    const std::ios_base::openmode log::READ_LOG_MODE = std::ios_base::in | std::ios_base::binary;

    bool log::exists(const std::string& filename)
    {        
        return utils::file::exists(filename);
    }
    
    void log::open(const std::string& filename)
    {
        __filename = filename;
        if(!utils::file::exists(filename))
            throw unknown_argument_exception(filename);
        
        __log_file->open(filename, OPEN_LOG_MODE);
        if(!__log_file->is_open())
            throw io_failure_exception("could not open table structure");
    }
    
    log::log()
    {
        __log_file = std::make_shared<std::fstream>();
        __wrt_lock = std::make_shared<std::mutex>();
    }

    log::log(const std::string& filename)
    {
        __filename = filename;        
        if(utils::file::exists(filename))
            throw already_exists_exception(filename);
        
        __log_file = std::make_shared<std::fstream>(filename, OPEN_LOG_MODE);
        __wrt_lock = std::make_shared<std::mutex>();
        if(!__log_file->is_open()) 
        {
            throw io_failure_exception("could not open table structure");
        }
    }

    log::~log() 
    {
        if(__log_file->is_open() && __log_file.use_count() == 1)
            __log_file->close();
        
    }
    
    void log::delete_log()
    {
        __log_file->close();        
        if(!utils::file::exists(__filename))
            return;
        
        std::string dir = utils::file::extract_directoy(__filename);
        std::string file = utils::file::extract_filename(__filename);
        file[0] = 'r';
        
        std::string new_filename = utils::file::merge_filename(dir, file);
        std::rename(__filename.c_str(), new_filename.c_str());
    }
    
    bool log::is_open() const
    {
        return __log_file->is_open();
    }
    
    void log::close() 
    {
        __log_file->close();
    }

    uint32_t log::put(const std::string& key, const std::string& value)
    {     
        data::encoder data_encoder;
        data::blob encoded_pair = data_encoder.encode(std::make_pair(key, value));
        
        __wrt_lock.get()->lock();
        size_t pre_write_index = __log_file->tellp();
        __log_file->write(encoded_pair.data, encoded_pair.length);
        if(__log_file->fail())
            throw io_failure_exception();
        
        __wrt_lock.get()->unlock();
        return pre_write_index;
    }
    
    void log::get(uint32_t offset, std::string& value) const 
    {        
        try {
            data::decoder_iterator<std::string, std::string> itr(__log_file, offset);
            value = itr.next().second;
        } 
        catch(const std::exception& e) {
            throw io_failure_exception();
        }
    }
    
    void log::remove(const std::string& key)
    {
        put(key, TOMBSTONE_TOKEN);
    }
    
    log::iterator log::begin() const
    {
        std::shared_ptr<std::istream> read_ptr = std::make_shared<std::ifstream>(__filename, READ_LOG_MODE);
        return iterator(read_ptr);
    }

}



