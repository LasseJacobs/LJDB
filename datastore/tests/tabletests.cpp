
/*
 * File:   tabletests.cpp
 * Author: lasse
 *
 * Created on Jul 6, 2019, 6:13:43 PM
 */

#include <fstream>
#include <dirent.h>
#include <cstdio>
#include <ctime>
#include <stdlib.h>     /* srand, rand */

#include <iostream>

#include "tabletests.h"
#include "../dbimp.h"
#include "../utils.h"
#include "../database_exceptions.h"

#define TEST_DIR "namespace/"

using namespace core;


CPPUNIT_TEST_SUITE_REGISTRATION(tabletests);

tabletests::tabletests() {
      srand(0);
}

tabletests::~tabletests() {
}

void tabletests::setUp() {
}

void tabletests::tearDown() {
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

void tabletests::test_create_table() {
    const std::string name = "cubes";
    const std::string table_template = "{edge:int}";
    core::db_imp dbImp(TEST_DIR, "testDb");
    dbImp.create_table(name);
    if (!exists(name)) {
        CPPUNIT_ASSERT(false);
    }
}

void tabletests::test_try_create_existing_table() {
    const std::string name = "cubes_II";
    const std::string table_template = "{edge:int}";
    core::db_imp dbImp(TEST_DIR, "testDb");
    dbImp.create_table(name);
    CPPUNIT_ASSERT_THROW(dbImp.create_table(name), core::already_exists_exception);
}

void tabletests::test_remove_table() {
    const std::string name = "cubes";
    const std::string table_template = "{edge:int}";
    core::db_imp dbImp(TEST_DIR, "testDb");
    dbImp.create_table(name);
    dbImp.remove_table(name);
    CPPUNIT_ASSERT_MESSAGE("table still exists after remove", !exists(name));
}

void tabletests::test_put_value() {
    const std::string name = "tweets";
    const std::string table_template = "{id:tweet}";
    
    const std::string key = "004";
    const std::string value = "hello web";
    
    core::db_imp dbImp(TEST_DIR, "testDb");
    dbImp.create_table(name);
    dbImp.put(name, "001", "Hello World!");
    dbImp.put(name, key, value);
    dbImp.put(name, "007", "Hello Web 2!");
    
    std::string got_value;
    dbImp.get(name, key, got_value);
    CPPUNIT_ASSERT_EQUAL(value, got_value);
}

void tabletests::test_get_value() {
    const std::string name = "tweets";
    const std::string table_template = "{id:tweet}";
    core::db_imp dbImp(TEST_DIR, "testDb");
    dbImp.create_table(name);
    
    const std::string key = "001";
    const std::string value = "Hello World!";
    dbImp.put(name, key, value);
       
    std::string read_value;
    dbImp.get(name, key, read_value);
    CPPUNIT_ASSERT_EQUAL(value, read_value);
}

void tabletests::test_remove_value() {
    const std::string name = "tweets";
    const std::string table_template = "{id:tweet}";
    core::db_imp dbImp(TEST_DIR, "testDb");
    dbImp.create_table(name);
    
    const std::string key = "R001";
    const std::string value = "Hello World!";
    dbImp.put(name, key, value);
    dbImp.remove(name, key);
    
    std::string read_value;
    CPPUNIT_ASSERT_THROW(dbImp.get(name, key, read_value), core::expected_exception);
}


bool tabletests::exists (const std::string& name) {
    std::string filename = utils::string::format(table_log_format, name);
    filename = utils::file::merge_filename(TEST_DIR, filename);
        
    std::ifstream f(filename);
    return f.good();
}

std::vector<std::string> tabletests::get_file_list(const std::string& path)
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
