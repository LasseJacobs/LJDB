
/*
 * File:   databasetests.cpp
 * Author: lasse
 *
 * Created on Aug 3, 2019, 6:08:30 PM
 */

#include "databasetests.h"

#include "../database_manager.h"

#define TEST_DIR "namespace/"


CPPUNIT_TEST_SUITE_REGISTRATION(databasetests);

databasetests::databasetests() {
}

databasetests::~databasetests() {
}

void databasetests::setUp() {
}

void databasetests::tearDown() {
}

void databasetests::test_creation() 
{
    core::database_manager dbm(TEST_DIR);
    dbm.create_database("testDb");
    
    CPPUNIT_ASSERT(true);
}

void databasetests::test_loading() 
{
    core::database_manager dbm(TEST_DIR);
    dbm.load_database("testDb");
    
    CPPUNIT_ASSERT(dbm.current_database()->database_name() == "testDb");
}

void databasetests::test_remove() 
{
    std::string db_name = "removableDb";
    
    core::database_manager dbm(TEST_DIR);
    dbm.create_database(db_name);
    
    dbm.remove_database(db_name);
    CPPUNIT_ASSERT_THROW(dbm.load_database(db_name), std::invalid_argument);
}

