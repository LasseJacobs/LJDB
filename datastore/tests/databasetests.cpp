
/*
 * File:   databasetests.cpp
 * Author: lasse
 *
 * Created on Aug 3, 2019, 6:08:30 PM
 */

#include "databasetests.h"

#include "../database_manager.h"
#include "../database_exceptions.h"

#include <dirent.h>
#include <cstdio>
#include <ctime>
#include <stdlib.h>     /* srand, rand */

#define TEST_DIR "namespace/"


CPPUNIT_TEST_SUITE_REGISTRATION(databasetests);

databasetests::databasetests() {
}

databasetests::~databasetests() {
}

void databasetests::setUp() {
}

void databasetests::tearDown() 
{
    try 
    {
        core::database_manager dbm(TEST_DIR);
        dbm.remove_database(_test_db_name);
    }
    catch(std::exception& e)
    {
        
    }
    
    std::vector<std::string> file_list = get_file_list(TEST_DIR);
    for (auto const& filename : file_list) {
        if(filename[0] == 'v' || filename[0] == 'r') {
            std::ostringstream oss;
            oss << std::time(0) << "-" << rand() % 1000000 << ".dump";
            std::rename(utils::file::merge_filename(TEST_DIR, filename).c_str(),
                    utils::file::merge_filename(TEST_DIR, oss.str()).c_str());
        }
    }
}

void databasetests::test_creation() 
{
    core::database_manager dbm(TEST_DIR);
    dbm.create_database(_test_db_name);
    
    CPPUNIT_ASSERT(true);
}

void databasetests::test_loading() 
{
    core::database_manager dbm(TEST_DIR);
    dbm.create_database(_test_db_name);
    dbm.load_database(_test_db_name);
    
    CPPUNIT_ASSERT(dbm.current_database()->database_name() == _test_db_name);
}

void databasetests::test_remove() 
{
    std::string db_name = "removableDb";
    
    core::database_manager dbm(TEST_DIR);
    dbm.create_database(db_name);
    
    dbm.remove_database(db_name);
    CPPUNIT_ASSERT_THROW(dbm.load_database(db_name), core::unknown_argument_exception);
}

std::vector<std::string> databasetests::get_file_list(const std::string& path)
{
    std::vector<std::string> file_list;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (path.c_str())) != NULL) {
    /* print all the files and directories within directory */
    while ((ent = readdir (dir)) != NULL) {
        file_list.push_back(ent->d_name);
    }
    closedir (dir);
    } else {
        /* could not open directory */
        perror ("");
    }
    return file_list;
}