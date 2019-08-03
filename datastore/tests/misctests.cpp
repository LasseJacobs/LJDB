
/*
 * File:   misctests.cpp
 * Author: lasse
 *
 * Created on Jul 20, 2019, 6:15:07 PM
 */

#include "misctests.h"
#include <iostream>
#include <cstring>

#define TEST_DIR "namespace/"


CPPUNIT_TEST_SUITE_REGISTRATION(misctests);


template<typename T>
int convert_type_to_enum(T type)
{
    if(std::is_integral<T>::value)
        return (std::is_signed<T>::value) ? 0 : 1;
    else if (std::is_floating_point<T>::value)
        return 2;
    else
        throw std::invalid_argument("unknown type");
}


misctests::misctests() {
}

misctests::~misctests() {
}

void misctests::setUp() {
}

void misctests::tearDown() {
}

void misctests::test_basic() 
{
    long capacity = 16;
    char bytes[16];
    long offset = 4;
    long bytes_written = 4;
       
    CPPUNIT_ASSERT_EQUAL_MESSAGE("substraction failed", capacity, &bytes[capacity-1] - &bytes[0] + 1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("offset failed", &bytes[offset], bytes + offset);
    
    char* head = bytes + bytes_written;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("size failed", bytes_written, head - bytes);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("remaining failed", capacity - bytes_written, (bytes + capacity) - head);
    
    //CPPUNIT_ASSERT(true);
}

void misctests::test_basic_II() 
{
    char* random_pointer = nullptr;
    delete[] random_pointer;
}

void misctests::test_basic_III() 
{
    std::cout << typeid(uint32_t).name() << std::endl;
    std::cout << typeid(uint16_t).name() << std::endl;
    std::cout << typeid(uint).name() << std::endl;
    
    std::string a_string = "world";
    std::cout << "int: " << convert_type_to_enum(5) << std::endl;
    std::cout << "u_int: " << convert_type_to_enum(5U) << std::endl;
}

struct test_pair
{
    uint16_t first:12;
    uint8_t second:4;
};

void misctests::test_basic_IV()
{
    std::cout << "size of pair: " << sizeof(test_pair) << std::endl;
    unsigned char binary_pair[4] = {0b00101010, 0b00000010, 0b00001010, 0b00000000 };
    
    std::pair<uint16_t, uint16_t> non_b_pair;
    non_b_pair.first = 554;
    non_b_pair.second = 10;
    std::memcpy(&binary_pair, &non_b_pair, 4);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("read failed 1", (uint16_t)554, non_b_pair.first);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("read failed 2", (uint16_t)10, non_b_pair.second);

}

void misctests::test_basic_V()
{
    unsigned char binary_pair[2] = {0b00101010, 0b00000000};
    
    uint16_t my_int;
    std::memcpy(&my_int, &binary_pair, 2);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("read failed 1", (uint16_t)42, my_int);
}

void misctests::test_basic_VI()
{
    std::string tombstone("\0\0\0\0", 4);
    CPPUNIT_ASSERT_EQUAL(4UL, tombstone.size());
}

void misctests::test_io_I()
{
    
}

