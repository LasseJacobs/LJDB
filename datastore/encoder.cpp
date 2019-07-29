
/* ENCODING
+----------------|----------------|----------------||||----------------+
||1a||*************1b************|||**||**************4****************|
+----------------|----------------|----------------||||----------------+

+----------------|----------------|-----------------------+
||||**************2b*************|||**** 3 - variable ****|
+----------------|----------------|------*----------------+
 
 * Table:
 *      [0:3] 1a: type of record
 *      [4:15] 2b: number of attributes 
 *      [16:19] RESERVED
 *      [20: 47] total length (redundant)
 * 
 *      [0:1] 2a: type of attribute
 *      [2:15] 2b: size of attribute
 *      [16:XX] 3: variable sized attribute
*/

#include "encoder.h"
#include <cstring>
#include <stdlib.h>
#include <stdexcept>
#include <cmath>


namespace data
{   
    
    template<typename K, typename V>
    blob encoder::encode(std::pair<K, V> record)
    {
        __buffer.clear();
        
        first_head_field header_1;
        second_head_field header_2;

        header_1.type = record_type::BASIC;
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

    template<typename T>
    size_t encoder::__encode_attribute(T attribute)
    {
        attribute_type_field header;
        header.type = convert_numeral_to_enum(attribute);
        header.size = sizeof(attribute);
        __buffer.append_copy(&header, sizeof(header));
        __buffer.append_copy(&attribute, sizeof(attribute));
        
        return sizeof(header) + sizeof(attribute);
    }
    
    template<>
    size_t encoder::__encode_attribute(std::string attribute)
    {
        attribute_type_field header;
        header.type = (uint8_t)attribute_type::STRING;
        header.size = attribute.size();
        __buffer.append_copy(&header, sizeof(header));
        __buffer.append_copy(&attribute, attribute.size());
        
        return sizeof(header) + attribute.size();
    }
}
