
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
    core::DbImp dbImp(TEST_DIR);
    Status result = dbImp.create_table(name, table_template);
    if (result != Status::SUCCESS || !exists(name)) {
        CPPUNIT_ASSERT(false);
    }
}

void tabletests::test_try_create_existing_table() {
    const std::string name = "cubes";
    const std::string table_template = "{edge:int}";
    core::DbImp dbImp(TEST_DIR);
    Status result = dbImp.create_table(name, table_template);
    Status result2 = dbImp.create_table(name, table_template);
    if (result != Status::SUCCESS || result2 == Status::SUCCESS ) {
        CPPUNIT_ASSERT(false);
    }
}

void tabletests::test_rename_table() {
    const std::string name = "cubes";
    const std::string new_name = "squares";
    const std::string table_template = "{edge:int}";
    core::DbImp dbImp(TEST_DIR);
    Status result = dbImp.create_table(name, table_template);
    Status result2 = dbImp.rename_table(name, new_name);
    if (result != Status::SUCCESS || !exists(new_name) ) {
        CPPUNIT_ASSERT(false);
    }
}

void tabletests::test_remove_table() {
    const std::string name = "cubes";
    const std::string table_template = "{edge:int}";
    core::DbImp dbImp(TEST_DIR);
    Status result = dbImp.create_table(name, table_template);
    Status result2 = dbImp.remove_table(name);
    if (result != Status::SUCCESS || exists(name) ) {
        CPPUNIT_ASSERT(false);
    }
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