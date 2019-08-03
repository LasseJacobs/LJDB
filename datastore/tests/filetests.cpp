
/*
 * File:   filetests.cpp
 * Author: lasse
 *
 * Created on Jul 12, 2019, 4:48:27 PM
 */

#include "filetests.h"
#include <fstream>
#include <cstring>


#define TEST_DIR "namespace/"


CPPUNIT_TEST_SUITE_REGISTRATION(filetests);

filetests::filetests() {
}

filetests::~filetests() {
}

void filetests::setUp() {
}

void filetests::tearDown() {
}

void filetests::test_write_read() 
{
    std::string filename = TEST_DIR;
    filename = filename + "v_test.txt"; 
    std::fstream out(filename, std::ios::in | std::ios::out | std::ios::app | std::ios::ate | std::ios::binary);
    std::fstream in1(filename, std::ios::in | std::ios::out | std::ios::binary);
    //std::ifstream in2(filename);
    CPPUNIT_ASSERT_MESSAGE("failed to open files", out.is_open() && in1.is_open() /*&& in2.is_open()*/);

    char out_buffer[100] = {"test"}; 
    out.write(out_buffer, 100);
    out.flush();
    CPPUNIT_ASSERT_MESSAGE("failed to write out", !out.fail());
    
    char in_buffer[100];
    in1.seekg(0);
    in1.read(in_buffer, 50);
    CPPUNIT_ASSERT_MESSAGE("failed to read", !in1.fail());

    CPPUNIT_ASSERT_MESSAGE("did not read the same text", std::strcmp(in_buffer, "test") == 0);
}

void filetests::test_replace_write()
{
    std::string filename = TEST_DIR;
    filename = filename + "v_test_2.txt"; 
    std::ofstream out(filename, std::ios::binary);
    std::ifstream in1(filename, std::ios::binary);
    CPPUNIT_ASSERT_MESSAGE("failed to open files", out.is_open() && in1.is_open());
    
    char out_buffer[20] = {"hwwlo world"}; 
    out.write(out_buffer, 20);
    out.flush();
    CPPUNIT_ASSERT_MESSAGE("failed to write out", !out.fail());
    
    char out_buffer_2[20] = {"el"};
    out.seekp(1);
    out.write(out_buffer_2, 2);
    out.flush();
    CPPUNIT_ASSERT_MESSAGE("failed to write out", !out.fail());
    
    char in_buffer[100];
    in1.read(in_buffer, 20);
    CPPUNIT_ASSERT_MESSAGE("failed to read", !in1.fail());
    
    std::cout << "I read: " << in_buffer << std::endl;
    CPPUNIT_ASSERT_MESSAGE("did not read the same text", std::strcmp(in_buffer, "hello world") == 0);
}
