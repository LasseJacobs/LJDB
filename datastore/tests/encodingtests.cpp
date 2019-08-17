
/*
 * File:   encodingtests.cpp
 * Author: lasse
 *
 * Created on Jul 30, 2019, 10:36:03 AM
 */

#include "encodingtests.h"

#include <utility>
#include <string>
#include <istream>

#include "../span.h"
#include "../span_streambuf.h"
#include "../encoder.h"
#include "../decoder_iterator.h"


CPPUNIT_TEST_SUITE_REGISTRATION(encodingtests);

encodingtests::encodingtests() {
}

encodingtests::~encodingtests() {
}

void encodingtests::setUp() {
}

void encodingtests::tearDown() {
}

void encodingtests::test_encode_to_file()
{
    lsl::span encoded_data_span;
    
    std::vector<std::pair<std::string, std::string>> plain_pairs = {
        {"foo", "1"}, 
        {"bar", "2"}, 
        {"var", "3"}, 
        {"car", "4"}
    };
    
    data::encoder encoder;
    for(int i = 0; i < plain_pairs.size(); i++)
    {
        data::blob encoded_data = encoder.encode(plain_pairs[i]);
        encoded_data_span.append_copy(encoded_data.data, encoded_data.length);
    }
    
    std::ofstream out_file("__test.txt");
    out_file.write(encoded_data_span.begin_raw(), encoded_data_span.size());
}

void encodingtests::test_encode_decode() 
{
    data::encoder encoder;
    std::pair<std::string, std::string> plain_pair("foo", "1234");
    data::blob encoded_data = encoder.encode(plain_pair);
    
    lsl::span encoded_data_span;
    encoded_data_span.append_copy(encoded_data.data, encoded_data.length);
    
    lsl::span_streambuf buff(encoded_data_span);
    data::decoder_iterator<std::string, std::string> itr(std::make_shared<std::istream>(&buff));
    std::pair<std::string, std::string> decoded_pair = itr.next();
    
    CPPUNIT_ASSERT(plain_pair == decoded_pair);
}

void encodingtests::test_encode_decode_group()
{
    lsl::span encoded_data_span;
    
    std::vector<std::pair<std::string, std::string>> plain_pairs = {
        {"foo", "1"}, 
        {"bar", "2"}, 
        {"var", "3"}, 
        {"car", "4"}
    };
    
    data::encoder encoder;
    for(int i = 0; i < plain_pairs.size(); i++)
    {
        data::blob encoded_data = encoder.encode(plain_pairs[i]);
        encoded_data_span.append_copy(encoded_data.data, encoded_data.length);
    }
    
    lsl::span_streambuf buff(encoded_data_span);
    data::decoder_iterator<std::string, std::string> itr(std::make_shared<std::istream>(&buff));
    for(int i = 0; i < plain_pairs.size(); i++)
    {
        std::pair<std::string, std::string> decoded_pair = itr.next();
        CPPUNIT_ASSERT(plain_pairs[i] == decoded_pair);
    }
}

