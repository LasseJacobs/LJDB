
/*
 * File:   parsertest.cpp
 * Author: lasse
 *
 * Created on Aug 14, 2019, 6:39:01 PM
 */

#include "parsertest.h"

#include "../tokenizer.h"
#include "../command_parser.h"
#include "../span.h"
#include "../prepared_statement.h"
#include <exception>
#include <map>
#include <queue>


CPPUNIT_TEST_SUITE_REGISTRATION(parsertest);

parsertest::parsertest() {
}

parsertest::~parsertest() {
}

void parsertest::setUp() {
}

void parsertest::tearDown() {
}

void parsertest::test_method() 
{
    eval::tokenizer tokenizer;
    eval::command_parser parser;
    
    lsl::span buffer = "create database myDb";
    std::queue<eval::token_t> token_chain = tokenizer.tokenize(eval::text_iterator(buffer.begin(), buffer.end()));
    std::map<std::string, std::string> context;
    
    eval::prepared_statement stmt = parser.parse(token_chain, context);
    CPPUNIT_ASSERT_MESSAGE("failed to parse", !stmt.failed());
    CPPUNIT_ASSERT_MESSAGE("wrong command", eval::command_t::CREATE_DATABASE == stmt.command());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("wrong argument", std::string("myDb"), stmt.argument(0));
    CPPUNIT_ASSERT_THROW(stmt.argument(1), std::range_error);
}

