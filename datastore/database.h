
#ifndef LJDB_DB_INTERFACE_H
#define LJDB_DB_INTERFACE_H


#include <string>
#include "status.h"


namespace core {

class database {
    public:        
        virtual const std::string& database_name() const = 0;

        virtual void create_table(const std::string& name) = 0;
        virtual void remove_table(const std::string& name) = 0;

        virtual void get(const std::string& table, const std::string& key, std::string& value) const = 0;
        virtual void put(const std::string& table, const std::string& key, const std::string& value) = 0;
        virtual void remove(const std::string& table, const std::string& key) = 0;

};

}

#endif
