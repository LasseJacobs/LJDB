
/*
 * File:   databasetests.h
 * Author: lasse
 *
 * Created on Aug 3, 2019, 6:08:30 PM
 */

#ifndef DATABASETESTS_H
#define DATABASETESTS_H

#include <cppunit/extensions/HelperMacros.h>

class databasetests : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(databasetests);

    CPPUNIT_TEST(test_creation);
    CPPUNIT_TEST(test_loading);
    CPPUNIT_TEST(test_remove);

    CPPUNIT_TEST_SUITE_END();

public:
    databasetests();
    virtual ~databasetests();
    void setUp();
    void tearDown();

private:
    void test_creation();
    void test_loading();
    void test_remove();
};

#endif /* DATABASETESTS_H */

