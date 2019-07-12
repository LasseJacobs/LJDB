
/*
 * File:   filetests.h
 * Author: lasse
 *
 * Created on Jul 12, 2019, 4:48:26 PM
 */

#ifndef FILETESTS_H
#define FILETESTS_H

#include <cppunit/extensions/HelperMacros.h>

class filetests : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(filetests);

    CPPUNIT_TEST(test_write_read);
    CPPUNIT_TEST(test_replace_write);

    CPPUNIT_TEST_SUITE_END();

public:
    filetests();
    virtual ~filetests();
    void setUp();
    void tearDown();

private:
    void test_write_read();
    void test_replace_write();
};

#endif /* FILETESTS_H */

