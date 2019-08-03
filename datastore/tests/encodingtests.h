
/*
 * File:   encodingtests.h
 * Author: lasse
 *
 * Created on Jul 30, 2019, 10:36:03 AM
 */

#ifndef ENCODINGTESTS_H
#define ENCODINGTESTS_H

#include <cppunit/extensions/HelperMacros.h>

class encodingtests : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(encodingtests);
    
    //CPPUNIT_TEST(test_encode_to_file);

    CPPUNIT_TEST(test_encode_decode);
    CPPUNIT_TEST(test_encode_decode_group);

    CPPUNIT_TEST_SUITE_END();

public:
    encodingtests();
    virtual ~encodingtests();
    void setUp();
    void tearDown();

private:
    void test_encode_to_file();
    
    void test_encode_decode();
    void test_encode_decode_group();

};

#endif /* ENCODINGTESTS_H */

