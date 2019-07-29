
/* 
 * File:   data.h
 * Author: lasse
 *
 * Created on July 26, 2019, 4:27 PM
 */

#ifndef DATA_H
#define DATA_H

#include <utility>
#include <stdlib.h>
#include <stdexcept>

namespace data 
{

    struct blob {
        char* data;
        size_t length;
    };

    enum class record_type : uint8_t
    {
        BASIC = 0
    };

    enum class attribute_type : uint8_t
    {
        STRING = 0,
        INTEGER = 1,
        U_INTEGER = 2,
        FLOAT = 3
    };

    struct first_head_field 
    {
        uint8_t type:4;
        uint16_t attributes:12;
    };
    struct attribute_type_field 
    {
        uint8_t type:2;
        uint16_t size:14;
    };
    struct second_head_field 
    {
        uint8_t RESERVED:4;
        uint32_t total_size:28;
    };
    
    template<typename T>
    int convert_numeral_to_enum(T type);

}

#endif /* DATA_H */

