
/*
 * File:   misctests.h
 * Author: lasse
 *
 * Created on Jul 20, 2019, 6:15:07 PM
 */

#ifndef MISCTESTS_H
#define MISCTESTS_H

#include <cppunit/extensions/HelperMacros.h>

class misctests : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(misctests);

    CPPUNIT_TEST(test_basic);
    CPPUNIT_TEST(test_basic_II);
    CPPUNIT_TEST(test_basic_III);
    CPPUNIT_TEST(test_basic_IV);
    CPPUNIT_TEST(test_basic_V);
    CPPUNIT_TEST(test_basic_VI);

    //CPPUNIT_TEST(test_io_I);
    


    CPPUNIT_TEST_SUITE_END();

public:
    misctests();
    virtual ~misctests();
    void setUp();
    void tearDown();

private:
    void test_basic();
    void test_basic_II();
    void test_basic_III();
    
    void test_basic_IV();
    void test_basic_V();
    void test_basic_VI();
    
    void test_io_I();
};

#endif /* MISCTESTS_H */

