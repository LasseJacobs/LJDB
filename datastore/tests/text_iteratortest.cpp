
/*
 * File:   text_iteratortest.cpp
 * Author: lasse
 *
 * Created on Aug 11, 2019, 5:04:14 PM
 */

#include "text_iteratortest.h"

#include "../span.h"
#include "../text_iterator.h"
#include <iostream>


CPPUNIT_TEST_SUITE_REGISTRATION(text_iteratortest);

text_iteratortest::text_iteratortest() {
}

text_iteratortest::~text_iteratortest() {
}

void text_iteratortest::setUp() {
}

void text_iteratortest::tearDown() {
}

void text_iteratortest::test_read() 
{
    lsl::span buffer = "this is a sentence";
    eval::text_iterator it(buffer.begin(), buffer.end());
    
    int i = 0;
    while(it.has_next()) {
        std::cout << it.next_word() << std::endl;
        i++;
    }
    
    CPPUNIT_ASSERT(i == 4);
}

void text_iteratortest::test_value_read()
{
    lsl::span buffer = "  this is \"a sentence\" it is";
    eval::text_iterator it(buffer.begin(), buffer.end());
    
    int i = 0;
    while(it.has_next()) {
        std::cout << it.next_word() << std::endl;
        i++;
    }
}

void text_iteratortest::test_empty_value_read()
{
    lsl::span buffer = "this is \"a sentence\" it \"\" word";
    eval::text_iterator it(buffer.begin(), buffer.end());
    
    int i = 0;
    while(it.has_next()) {
        std::cout << it.next_word() << std::endl;
        i++;
    }
}