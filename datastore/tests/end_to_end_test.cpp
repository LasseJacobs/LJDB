
/*
 * File:   end_to_end_test.cpp
 * Author: lasse
 *
 * Created on Aug 23, 2019, 12:12:30 PM
 */

#include "end_to_end_test.h"
#include <sstream>

#include <dirent.h>
#include <cstdio>
#include <ctime>
#include <stdlib.h>     /* srand, rand */

#define TEST_DIR "namespace/"


CPPUNIT_TEST_SUITE_REGISTRATION(end_to_end_test);

end_to_end_test::end_to_end_test() : interpreter(tout, terr) {
}

end_to_end_test::~end_to_end_test() 
{
}

void end_to_end_test::setUp() 
{
}

void end_to_end_test::tearDown() 
{
    std::vector<std::string> file_list = get_file_list(TEST_DIR);
    for (auto const& filename : file_list) {
        if(filename[0] == 't' || filename[0] == 'r') {
            std::ostringstream oss;
            oss << std::time(0) << "-" << rand() % 1000000 << ".dump";
            std::rename(utils::file::merge_filename(TEST_DIR, filename).c_str(),
                    utils::file::merge_filename(TEST_DIR, oss.str()).c_str());
        }
    }
}

void end_to_end_test::test_flow_I() 
{
    LJDBFacade database(std::string(TEST_DIR));
    execute_command(database, "create database test1Db");
    execute_command(database, "select database test1Db");
    
    execute_command(database, "create table plain");
    
    execute_command(database, "put plain \"key\" \"value\"");
    std::string r = execute_command(database, "get plain \"key\"");
    CPPUNIT_ASSERT_EQUAL_MESSAGE("result did not match", std::string("value"), r);

    execute_command(database, "remove plain \"key\"");
    execute_command_fail(database, "get plain \"key\"");

    execute_command(database, "exit");

    CPPUNIT_ASSERT(true);
}

std::string end_to_end_test::execute_command(LJDBFacade& database, const std::string& cmd)
{
    tout.clear();
    tout.str("");
    
    __execute_command_imp(database, cmd);
    std::string output = tout.str();
    if(output.size() >= interpreter.RESULT_PREFIX.size() &&
            output.substr(0, interpreter.RESULT_PREFIX.size()) == interpreter.RESULT_PREFIX)
    {
        std::size_t result_size = output.size() - interpreter.RESULT_PREFIX.size() - 1;
        return output.substr(interpreter.RESULT_PREFIX.size(), result_size);
    }
    else if(output != "")
    {
        CPPUNIT_ASSERT_MESSAGE(output, false);
    }
    
    return output;
}

void end_to_end_test::execute_command_fail(LJDBFacade& database, const std::string& cmd) 
{
    tout.clear();
    tout.str("");
    
    __execute_command_imp(database, cmd);
    std::string output = tout.str();
    if(output.size() >= interpreter.RESULT_PREFIX.size() &&
            output.substr(0, interpreter.RESULT_PREFIX.size()) == interpreter.RESULT_PREFIX)
    {
        CPPUNIT_ASSERT_MESSAGE("expected failure, got result", false);
    }
    else if(output != "")
    {
        CPPUNIT_ASSERT(true);
        return;
    }
    
    CPPUNIT_ASSERT_MESSAGE("expected failure, got nothing", false);
}


void end_to_end_test::__execute_command_imp(LJDBFacade& database, const std::string& cmd) 
{
    lsl::span input_buffer = cmd.c_str();
    auto tokens = tokenizer.tokenize(eval::text_iterator(input_buffer.begin(), input_buffer.end()));
    auto stmt = parser.parse(tokens, context_map);
    try
    {
        interpreter.eval(stmt, database, context_map);
    }
    catch(const core::expected_exception& e)
    {
        tout << "failed: " << e.what() << std::endl;
    }
    catch(const std::exception& e)
    {
        terr << "an exception occurred...\n";
        terr << "CAUSE: " << e.what() << std::endl;
        CPPUNIT_ASSERT_MESSAGE(terr.str() + " on " + cmd, false);
    }
}


std::vector<std::string> end_to_end_test::get_file_list(const std::string& path)
{
    std::vector<std::string> file_list;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (path.c_str())) != NULL) {
    /* print all the files and directories within directory */
    while ((ent = readdir (dir)) != NULL) {
        file_list.push_back(ent->d_name);
    }
    closedir (dir);
    } else {
        /* could not open directory */
        perror ("");
    }
    return file_list;
}