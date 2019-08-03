
/* 
 * File:   dbfacade.h
 * Author: lasse
 *
 * Created on August 3, 2019, 11:18 AM
 */

#ifndef DBFACADE_H
#define DBFACADE_H

#include <string>
#include "status.h"


class LJDBFacade {
    public:
        
        virtual core::Status create_database(const std::string& name) = 0;
        virtual core::Status load_database(const std::string& name) = 0;
        virtual core::Status remove_database(const std::string& name) = 0;

        virtual core::Status create_table(const std::string& name) = 0;
        virtual core::Status remove_table(const std::string& name) = 0;

        virtual core::Status get(const std::string& table, const std::string& key, std::string& value) const = 0;
        virtual core::Status put(const std::string& table, const std::string& key, const std::string& value) = 0;
        virtual core::Status remove(const std::string& table, const std::string& key) = 0;

};

#endif /* DBFACADE_H */

