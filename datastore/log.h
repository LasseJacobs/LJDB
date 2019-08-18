
/* 
 * File:   log.h
 * Author: lasse
 *
 * Created on August 17, 2019, 10:58 AM
 */

#ifndef LOG_H
#define LOG_H

#include<string>
#include<memory>
#include <mutex>

#include "decoder_iterator.h"

namespace core {

class log {
public:
    typedef data::decoder_iterator<std::string, std::string> iterator;
    static const std::string TOMBSTONE_TOKEN;

   
    static bool exists(const std::string& filename);

    void open(const std::string& filename);
    log();
    log(const std::string& filename);

    ~log();

    void delete_log();

    bool is_open() const;
    
    void close();

    uint32_t put(const std::string& key, const std::string& value);
    void get(uint32_t offset, std::string& value) const;
    void remove(const std::string& key);

    iterator begin() const;
    
private:
    static const std::ios_base::openmode NEW_LOG_MODE;
    static const std::ios_base::openmode OPEN_LOG_MODE;
    static const std::ios_base::openmode READ_LOG_MODE;

    std::string __filename;
    std::shared_ptr<std::mutex> __wrt_lock;
    std::shared_ptr<std::fstream> __log_file;
};

}

#endif /* LOG_H */

