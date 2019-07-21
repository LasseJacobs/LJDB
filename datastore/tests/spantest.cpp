
/*
 * File:   spantest.cpp
 * Author: lasse
 *
 * Created on Jul 21, 2019, 11:37:41 AM
 */

#include "spantest.h"
#include "../span.h"

#include <cstring>
#include <stdexcept>


CPPUNIT_TEST_SUITE_REGISTRATION(spantest);

spantest::spantest() {
}

spantest::~spantest() {
}

void spantest::setUp() {
}

void spantest::tearDown() {
}

void spantest::test_regular_copy() 
{
    lsl::span span;
    
    const std::size_t size = 10; 
    char buffer[size] = "foo";
    span.copy(buffer, size);
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE("size is not right", size, span.size());
    CPPUNIT_ASSERT(std::strcmp(span.begin(), buffer) == 0);
}

void spantest::test_big_copy()
{
    lsl::span span;
    
    const std::size_t size = 100; 
    char buffer[size] = "this is a very long message that hopefully requires multiple resizes";
    span.copy(buffer, size);
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE("size is not right", size, span.size());
    CPPUNIT_ASSERT(std::strcmp(span.begin(), buffer) == 0);
}

void spantest::test_clear()
{
    lsl::span span;
    
    const std::size_t size = 100; 
    char buffer[size] = "this is a very long message that hopefully requires multiple resizes";
    span.copy(buffer, size);
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE("size is not right", size, span.size());
    span.clear(span.begin() + 10);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("size is not right", 10UL, span.size());
}

void spantest::test_false_clear()
{
    lsl::span span;
    
    const std::size_t size = 100; 
    char buffer[size] = "this is a very long message that hopefully requires multiple resizes";
    span.copy(buffer, size);
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE("size is not right", size, span.size());
    CPPUNIT_ASSERT_THROW(span.clear(buffer), std::range_error);
}

void spantest::test_clear_all()
{
    lsl::span span;
    
    const std::size_t size = 100; 
    char buffer[size] = "this is a very long message that hopefully requires multiple resizes";
    span.copy(buffer, size);
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE("size is not right", size, span.size());
    span.clear();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("size is not right", 0UL, span.size());
}

