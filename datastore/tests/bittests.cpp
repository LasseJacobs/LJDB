
/*
 * File:   bittests.cpp
 * Author: lasse
 *
 * Created on Jul 19, 2019, 11:18:08 AM
 */

#include "bittests.h"
#include <iostream>


CPPUNIT_TEST_SUITE_REGISTRATION(bittests);


struct field_1 
{
    u_int8_t type:4;
    u_int16_t attributes:12;
};
struct field_2 
{
    u_int8_t type:2;
    u_int16_t size:14;
};
struct field_4 
{
    u_int8_t reserved:4;
    u_int32_t total_size:28;
};

enum attribute_type : uint8_t
{
    STRING = (1 << 0),
    INTEGER = (1 << 1)
};

bittests::bittests() {
}

bittests::~bittests() {
}

void bittests::setUp() {
}

void bittests::tearDown() {
}

void bittests::test_sizes() 
{    
    std::cout << "size of field_1: " << sizeof(field_1) << " bytes" << std::endl;
    std::cout << "size of field_2: " << sizeof(field_2) << " bytes" << std::endl;
    std::cout << "size of field_4: " << sizeof(field_4) << " bytes" << std::endl;

    CPPUNIT_ASSERT_EQUAL(2UL, sizeof(field_1));
    CPPUNIT_ASSERT_EQUAL(2UL, sizeof(field_2));
    CPPUNIT_ASSERT_EQUAL(4UL, sizeof(field_4));
}

void bittests::test_assignment() 
{
    field_1 field;
    field.type = attribute_type::STRING;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("first assignment failed", (u_int8_t)0b00000001, (u_int8_t)field.type);
    
    field.attributes = 42;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("second assignment failed to retain first", (u_int8_t)0b00000001, (u_int8_t)field.type);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("second assignment failed to retain first", (u_int32_t)42, (u_int32_t)field.attributes);
}

void bittests::test_types()
{
    //typeid operator (see cpp reference)
    int myint = 42;
    std::cout << "type: " << typeid(myint).name() << std::endl;
    
    std::string mystring = "42";
    std::cout << "type: " << typeid(mystring).name() << std::endl;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("comparison failed", typeid(int).name(), typeid(myint).name());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("complex comparison failed", typeid(int32_t).name(), typeid(myint).name());

    
    unsigned short myshort = 42;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("short incorrectly handled", 2, __do_type_stuff(myshort));
    u_int32_t myotherint = 64;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("int incorrectly handled", 4, __do_type_stuff(myotherint));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("string incorrectly handled", 42, __do_type_stuff(mystring));
}

int bittests::__do_type_stuff(u_int16_t number)
{
    return 2;
}

int bittests::__do_type_stuff(u_int32_t number)
{
    return 4;
}

int bittests::__do_type_stuff(std::string number)
{
    return 42;
}
