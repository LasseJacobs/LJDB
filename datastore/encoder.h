
/* 
 * File:   encoder.h
 * Author: lasse
 *
 * Created on July 18, 2019, 11:13 AM
 * NOTE: this class is _not_ thread-safe
 */

#ifndef ENCODER_H
#define ENCODER_H

#include <utility>
#include <cstring>
#include <stdlib.h>
#include <stdexcept>
#include <cmath>
#include "span.h"
#include "data.h"

namespace data 
{

class encoder {
public:
    
    template<typename K, typename V>
    blob encode(std::pair<K, V> record)
    {
        __buffer.clear();
        
        first_head_field header_1;
        second_head_field header_2;

        header_1.type = (uint8_t)record_type::BASIC;
        header_1.attributes = 2u;
        __buffer.append_copy(&header_1, sizeof(header_1));
        __buffer.skip(sizeof(header_2));
        
        std::size_t bytes_encoded = 0;
        bytes_encoded += __encode_attribute(record.first);
        bytes_encoded += __encode_attribute(record.second);
        
        header_2.total_size = bytes_encoded;
        __buffer.insert_copy(sizeof(header_1), &header_2, sizeof(header_2));
        
        return {__buffer.begin(), __buffer.size()};
    }  
    
private:
    lsl::span __buffer;
    
    template<typename T>
    size_t __encode_attribute(T attribute)
    {
        attribute_type_field header;
        header.type = convert_numeral_to_enum(attribute);
        header.size = sizeof(attribute);
        __buffer.append_copy(&header, sizeof(header));
        __buffer.append_copy(&attribute, sizeof(attribute));
        
        return sizeof(header) + sizeof(attribute);
    }
    
    size_t __encode_attribute(std::string attribute)
    {
        attribute_type_field header;
        header.type = (uint8_t)attribute_type::STRING;
        header.size = attribute.size();
        __buffer.append_copy(&header, sizeof(header));
        __buffer.append_copy(&attribute, attribute.size());
        
        return sizeof(header) + attribute.size();
    }
};

}
 
#endif /* ENCODER_H */

