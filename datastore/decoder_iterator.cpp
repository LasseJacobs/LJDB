
/* 
 * File:   decoder_iterator.cpp
 * Author: lasse
 * 
 * Created on July 26, 2019, 4:22 PM
 */
/*
#include <istream>

//#include "decoder_iterator.h"

namespace data
{
    
    template<typename K, typename V>
    const int decoder_iterator<K, V>::SINGLE_PASS_SIZE = 512;
    
    template<typename K, typename V>
    decoder_iterator(std::istream& in) 
    {
        __bin_size_of_current = 0;
        __raw_source = in;
    }
    
    template<typename K, typename V>    
    std::pair<K, V> current() const
    {
        return __current_data;
    }
    
    template<typename K, typename V> 
    std::pair<K, V> next()
    {
        __buffer.shift(__bin_size_of_current);
        while(__buffer.size() < sizeof(first_head_field) + sizeof(second_head_field))
        {
            if(__raw_source.eof())
                throw std::runtime_error("end of file reached");
            
            char local_buffer[SINGLE_PASS_SIZE];
            __raw_source.read(local_buffer, SINGLE_PASS_SIZE);
            if(__raw_source.fail())
                throw std::runtime_error("read failure");
            __buffer.append_copy(local_buffer, __raw_source.gcount());
        }
        
        second_head_field size_header;
        const char* reader = __buffer.begin() + sizeof(first_head_field);
        std::memcpy(&size_header, reader, sizeof(size_header));
        __bin_size_of_current = (std::size_t)size_header.total_size;
        reader += sizeof(second_head_field);
        
        attribute_type_field key_att_header;
        std::memcpy(&key_att_header, reader, sizeof(key_att_header));
        reader += sizeof(attribute_type_field);
        
        const char* key = reader;
        reader += key_att_header.size;
        
        attribute_type_field value_att_header;
        std::memcpy(&value_att_header, reader, sizeof(value_att_header));
        reader += sizeof(attribute_type_field);
        
        const char* value = reader;
        
        std::pair<K, V> decoded_pair;
        __unpack(key, key_att_header.size, decoded_pair.first);
        __unpack(key, key_att_header.size, decoded_pair.second);        
    }
    
    template<typename K, typename V> 
    bool has_next() const
    {
        return !__raw_source.eof();
    }
    
    
    template<typename K, typename V>
    template<typename T>
    void __unpack(const char* ptr, std::size_t size, T& data_out)
    {
        std::memcpy(&data_out, ptr, size);
    }
    
    template<typename K, typename V>
    void __unpack(const char* ptr, std::size_t size, std::string& data_out)
    {
        std::memcpy(&data_out, ptr, size);
    }
}

*/