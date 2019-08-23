
/*
 * File:   end_to_end_test.h
 * Author: lasse
 *
 * Created on Aug 23, 2019, 12:12:29 PM
 */

#ifndef END_TO_END_TEST_H
#define END_TO_END_TEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <string>
#include <vector>
#include "../command_parser.h"
#include "../interpreter.h"
#include "../database_exceptions.h"

class end_to_end_test : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(end_to_end_test);

    CPPUNIT_TEST(test_flow_I);

    CPPUNIT_TEST_SUITE_END();

public:
    end_to_end_test();
    virtual ~end_to_end_test();
    void setUp();
    void tearDown();

private:
    std::ostringstream tout;
    std::ostringstream terr;
    
    eval::tokenizer tokenizer;
    eval::command_parser parser;
    eval::interpreter interpreter;

    std::map<std::string, std::string> context_map;
    
    void test_flow_I();
    
    std::string execute_command(LJDBFacade& database, const std::string& cmd);
    void execute_command_fail(LJDBFacade& database, const std::string& cmd);
    
    void __execute_command_imp(LJDBFacade& database, const std::string& cmd);
    
    std::vector<std::string> get_file_list(const std::string& path);
};

#endif /* END_TO_END_TEST_H */

