
/*
 * File:   spantest.h
 * Author: lasse
 *
 * Created on Jul 21, 2019, 11:37:40 AM
 */

#ifndef SPANTEST_H
#define SPANTEST_H

#include <cppunit/extensions/HelperMacros.h>

class spantest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(spantest);

    CPPUNIT_TEST(test_regular_copy);
    CPPUNIT_TEST(test_big_copy);
    CPPUNIT_TEST(test_clear);
    CPPUNIT_TEST(test_clear_all);
    CPPUNIT_TEST(test_false_clear);
    CPPUNIT_TEST(test_multiple_copy);
    CPPUNIT_TEST(test_insert_copy);
    CPPUNIT_TEST(test_insert_out_copy);
    CPPUNIT_TEST(test_shift);
    
    CPPUNIT_TEST(test_stream_buffer);
    CPPUNIT_TEST(test_stream_eof);
    

    CPPUNIT_TEST_SUITE_END();

public:
    spantest();
    virtual ~spantest();
    void setUp();
    void tearDown();

private:
    void test_regular_copy();
    void test_big_copy();
    void test_multiple_copy();
    void test_insert_copy();
    void test_insert_out_copy();
    
    void test_shift();
    
    void test_stream_buffer();
    void test_stream_eof();
    
    void test_clear();
    void test_clear_all();
    void test_false_clear();
};

#endif /* SPANTEST_H */

