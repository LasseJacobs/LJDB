
/*
 * File:   spantest.cpp
 * Author: lasse
 *
 * Created on Jul 21, 2019, 11:37:41 AM
 */

#include "spantest.h"
#include "../span.h"
#include "../span_streambuf.h"

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
    span.append_copy(buffer, size);
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE("size is not right", size, span.size());
    CPPUNIT_ASSERT(std::strcmp(span.begin(), buffer) == 0);
}

void spantest::test_big_copy()
{
    lsl::span span;
    
    const std::size_t size = 100; 
    char buffer[size] = "this is a very long message that hopefully requires multiple resizes";
    span.append_copy(buffer, size);
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE("size is not right", size, span.size());
    CPPUNIT_ASSERT(std::strcmp(span.begin(), buffer) == 0);
}

void spantest::test_multiple_copy()
{
    lsl::span span;
    
    const std::size_t size = 8; 
    char buffer[size] = "bar";
    
    int repeat = 4;
    for(int i = 0; i < repeat; i++)
    {
        span.append_copy(buffer, size);
    }
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE("size is not right", 4 * size, span.size());
    CPPUNIT_ASSERT(std::strcmp(span.begin() + size, buffer) == 0);
}

void spantest::test_insert_copy()
{
    lsl::span span;
    
    const std::size_t size = 100; 
    char buffer[size] = "this is a very long message that hopefully requires multiple resizes";
    int offset = 10;
    span.insert_copy(offset, buffer, size);
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE("size is not right", offset + size, span.size());
    CPPUNIT_ASSERT(std::strcmp(span.begin() + offset, buffer) == 0);
}

void spantest::test_insert_out_copy()
{
    lsl::span span;
    
    const std::size_t size = 100; 
    char buffer[size] = "this is a very long message that hopefully requires multiple resizes";
    int offset = 50;
    span.insert_copy(offset, buffer, size);
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE("size is not right", offset + size, span.size());
    CPPUNIT_ASSERT(std::strcmp(span.begin() + offset, buffer) == 0);
}

void spantest::test_clear()
{
    lsl::span span;
    
    const std::size_t size = 100; 
    char buffer[size] = "this is a very long message that hopefully requires multiple resizes";
    span.append_copy(buffer, size);
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE("size is not right", size, span.size());
    span.clear(span.begin() + 10);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("size is not right", 10UL, span.size());
}

void spantest::test_false_clear()
{
    lsl::span span;
    
    const std::size_t size = 100; 
    char buffer[size] = "this is a very long message that hopefully requires multiple resizes";
    span.append_copy(buffer, size);
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE("size is not right", size, span.size());
    CPPUNIT_ASSERT_THROW(span.clear(buffer), std::range_error);
}

void spantest::test_clear_all()
{
    lsl::span span;
    
    const std::size_t size = 100; 
    char buffer[size] = "this is a very long message that hopefully requires multiple resizes";
    span.append_copy(buffer, size);
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE("size is not right", size, span.size());
    span.clear();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("size is not right", 0UL, span.size());
}

void spantest::test_stream_buffer()
{
    lsl::span span;
    
    const std::size_t size = 100; 
    char buffer[size] = "this is a very long message that hopefully requires multiple resizes";
    span.append_copy(buffer, size);

    lsl::span_streambuf ssbuf(span);
    std::istream in(&ssbuf);
    
    std::stringstream out;
    for(char c; in.get(c); )
        out << c;
    
    CPPUNIT_ASSERT(std::strcmp(buffer, out.str().c_str()) == 0);
}

void spantest::test_stream_eof()
{
    lsl::span span;
    
    const std::size_t size = 100; 
    char buffer[size] = "this is a very long message that hopefully requires multiple resizes";
    span.append_copy(buffer, size);

    lsl::span_streambuf ssbuf(span);
    std::istream in(&ssbuf);
    
    in.read(buffer, size+10);
    
    CPPUNIT_ASSERT(in.eof());
}
