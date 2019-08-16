
/*
 * File:   parsertest.h
 * Author: lasse
 *
 * Created on Aug 14, 2019, 6:39:00 PM
 */

#ifndef PARSERTEST_H
#define PARSERTEST_H

#include <cppunit/extensions/HelperMacros.h>

class parsertest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(parsertest);

    CPPUNIT_TEST(test_method);

    CPPUNIT_TEST_SUITE_END();

public:
    parsertest();
    virtual ~parsertest();
    void setUp();
    void tearDown();

private:
    void test_method();
};

#endif /* PARSERTEST_H */

