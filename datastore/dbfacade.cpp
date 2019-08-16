
#include "dbfacade.h"

LJDBFacade::LJDBFacade(const std::string& path) : __dbm(path)
{
    
}

void LJDBFacade::create_database(const std::string& name)
{
    __dbm.create_database(name);
}

void LJDBFacade::load_database(const std::string& name)
{
    __database = __dbm.load_database(name);
}

void LJDBFacade::remove_database(const std::string& name)
{
    __dbm.remove_database(name);
}

void LJDBFacade::create_table(const std::string& name)
{
    __database->create_table(name);
}

void LJDBFacade::remove_table(const std::string& name)
{
    __database->remove_table(name);
}

void LJDBFacade::get(const std::string& table, const std::string& key, std::string& value) const
{
    __database->get(table, key, value);
}

void LJDBFacade::put(const std::string& table, const std::string& key, const std::string& value)
{
    __database->put(table, key, value);
}

void LJDBFacade::remove(const std::string& table, const std::string& key)
{
    __database->remove(table, key);
}

