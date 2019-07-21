
/*
 * File:   bittests.h
 * Author: lasse
 *
 * Created on Jul 19, 2019, 11:18:08 AM
 */

#ifndef BITTESTS_H
#define BITTESTS_H

#include <cppunit/extensions/HelperMacros.h>
#include <string>

class bittests : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(bittests);

    CPPUNIT_TEST(test_sizes);
    CPPUNIT_TEST(test_assignment);
    CPPUNIT_TEST(test_types);


    CPPUNIT_TEST_SUITE_END();

public:
    bittests();
    virtual ~bittests();
    void setUp();
    void tearDown();

private:
    void test_sizes();
    void test_assignment();
    void test_types();
    
    int __do_type_stuff(u_int16_t number);
    int __do_type_stuff(u_int32_t number);
    int __do_type_stuff(std::string number);
};

#endif /* BITTESTS_H */

