
/* 
 * File:   table.h
 * Author: lasse
 *
 * Created on July 8, 2019, 8:45 PM
 */

#ifndef TABLE_H
#define TABLE_H

#include<string>
#include<memory>
#include<unordered_map>
#include<vector>

#include"status.h"


namespace core {

    class table {
    public:
        static bool exists(const std::string& root, const std::string& name);
        static table open(const std::string& root, const std::string& name);
        
        table(const std::string& root, const std::string& name);

        ~table();
        
        Status remove();
        
        bool is_open();
        
        //read write ops
        Status put(const std::string& key, const std::string& value);
        Status get(const std::string& key, std::string& value) const;
        Status remove(const std::string& key);
        
        
    private:
        table(const std::string& root, const std::string& name, bool __private);
        
        std::string __translate_table_name(const std::string& name) const;
        std::string __translate_table_name(const std::string& name, const std::string& format) const;
                
        static const std::string TABLE_LOG_FORMAT;
        static const std::string REMOVED_TABLE_LOG_FORMAT;
        static const std::string TOMBSTONE_TOKEN;
                
        std::string __root;
        std::string __table_name;
        std::shared_ptr<std::fstream> __table_file;
        
        std::unordered_map<std::string, uint32_t> __key_index;
        std::vector<std::string> __keys;
        int __count = 0;
    }; 
}

#endif /* TABLE_H */

