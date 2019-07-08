
#ifndef LJDB_DB_INTERFACE_H
#define LJDB_DB_INTERFACE_H


#include <string>
#include "status.h"


namespace core {

class LJDB {
    public:
        //Probably best to put these somewhere else
        //
        //virtual Status create_database(const std::string& name) = 0;
        //virtual Status remove_database(const std::string& name) = 0;

        virtual Status create_table(const std::string& name, const std::string& table_template) = 0;
        virtual Status rename_table(const std::string& name) = 0;
        virtual Status remove_table(const std::string& name) = 0;

        virtual Status get(const std::string& table, const std::string& key, std::string& value) = 0;
        virtual Status put(const std::string& table, const std::string& key, const std::string& value) = 0;
        virtual Status remove(const std::string& table, const std::string& key) = 0;

    protected:
        const std::string _table_log_format = "v_%s_log.table";


};

}

#endif
