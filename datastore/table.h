
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

#include"status.h"


namespace core {

    class table {
    public:
        static bool exists(const std::string& root, const std::string& name);
        static table open(const std::string& root, const std::string& name);
        
        table(const std::string& root, const std::string& name, const std::string& table_template);
        table(const table& other);
        ~table();
        
        Status remove();
        
        bool is_open();
        
        //read write ops
        Status put(const std::string& key, const std::string& value);
        Status get(const std::string& key, std::string& value) const;

        
    private:
        table(const std::string& root, const std::string& name);
        
        std::string __translate_table_name(const std::string& name) const;
        std::string __translate_table_name(const std::string& name, const std::string& format) const;
        
        static const std::string __table_log_format;
        static const std::string __removed_table_log_format;
        
        std::string __root;
        std::string __table_name;
        std::shared_ptr<std::fstream> __table_file;
    }; 
}

#endif /* TABLE_H */

