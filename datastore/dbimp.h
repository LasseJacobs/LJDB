
#ifndef LJDB_DB_IMP_H
#define LJDB_DB_IMP_H

#include <unordered_map>

#include "dbinterface.h"
#include "table.h"


namespace core {

    class DbImp : public LJDB {
        public:
            DbImp(const std::string& root);

            Status create_table(const std::string& name);
            Status remove_table(const std::string& name);

            Status get(const std::string& table_name, const std::string& key, std::string& value) const;
            Status put(const std::string& table_name, const std::string& key, const std::string& value);
            Status remove(const std::string& table_name, const std::string& key);

        private: 
            std::string __root;
            mutable std::unordered_map<std::string, table> __open_tables;
    };

}

#endif