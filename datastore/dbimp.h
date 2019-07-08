
#ifndef LJDB_DB_IMP_H
#define LJDB_DB_IMP_H


#include "dbinterface.h"


namespace core {

    class DbImp : public LJDB {
        public:
            DbImp(const std::string& root);

            Status create_table(const std::string& name, const std::string& table_template);
            Status rename_table(const std::string& old_name, const std::string& new_name);
            Status remove_table(const std::string& name);

            Status get(const std::string& table, const std::string& key, std::string& value);
            Status put(const std::string& table, const std::string& key, const std::string& value);
            Status remove(const std::string& table, const std::string& key);

        private:
            std::string __translate_table_name(const std::string& name);
            std::string __translate_table_name(const std::string& name, const std::string& format);

            
            std::string __root;

    };

}

#endif