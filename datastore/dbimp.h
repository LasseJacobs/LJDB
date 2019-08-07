
#ifndef LJDB_DB_IMP_H
#define LJDB_DB_IMP_H

#include <unordered_map>

#include "database.h"
#include "table.h"


namespace core {

    class db_imp : public database {
        public:
            db_imp(const std::string& root, const std::string& name);
            
            const std::string& database_name() const;

            void create_table(const std::string& name);
            void remove_table(const std::string& name);

            void get(const std::string& table_name, const std::string& key, std::string& value) const;
            void put(const std::string& table_name, const std::string& key, const std::string& value);
            void remove(const std::string& table_name, const std::string& key);

        private: 
            std::string __name;
            std::string __root;
            mutable std::unordered_map<std::string, table> __open_tables;
    };

}

#endif