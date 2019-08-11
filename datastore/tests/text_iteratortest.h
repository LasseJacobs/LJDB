
/*
 * File:   text_iteratortest.h
 * Author: lasse
 *
 * Created on Aug 11, 2019, 5:04:13 PM
 */

#ifndef TEXT_ITERATORTEST_H
#define TEXT_ITERATORTEST_H

#include <cppunit/extensions/HelperMacros.h>

class text_iteratortest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(text_iteratortest);

    CPPUNIT_TEST(test_read);
    CPPUNIT_TEST(test_value_read);
    CPPUNIT_TEST(test_empty_value_read);



    CPPUNIT_TEST_SUITE_END();

public:
    text_iteratortest();
    virtual ~text_iteratortest();
    void setUp();
    void tearDown();

private:
    void test_read();
    void test_value_read();
    void test_empty_value_read();

};

#endif /* TEXT_ITERATORTEST_H */

