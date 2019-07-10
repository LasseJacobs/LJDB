
/*
 * File:   tabletests.h
 * Author: lasse
 *
 * Created on Jul 6, 2019, 6:13:43 PM
 */

#ifndef TABLETESTS_H
#define TABLETESTS_H

#include <cppunit/extensions/HelperMacros.h>

class tabletests : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(tabletests);

    CPPUNIT_TEST(test_create_table);
    CPPUNIT_TEST(test_try_create_existing_table);
    CPPUNIT_TEST(test_remove_table);
    CPPUNIT_TEST(test_put_value);
    CPPUNIT_TEST(test_get_value);



    CPPUNIT_TEST_SUITE_END();

public:
    tabletests();
    virtual ~tabletests();
    void setUp();
    void tearDown();

private:
    void test_create_table();
    void test_try_create_existing_table();
    void test_remove_table();
    void test_put_value();
    void test_get_value();
    
    
    bool exists(const std::string& name);
    std::vector<std::string> get_file_list(const std::string& path);
    
    const std::string table_log_format = "v_%s_log.table";

};

#endif /* TABLETESTS_H */

