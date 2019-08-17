
/* 
 * File:   decoder_iterator.h
 * Author: lasse
 *
 * Created on July 26, 2019, 4:22 PM
 */

#ifndef DECODER_ITERATOR_H
#define DECODER_ITERATOR_H

#include <istream>
#include <iostream>
#include <memory>


#include "data.h"
#include "span.h"


namespace data
{
    
template<typename K, typename V>
class decoder_iterator {
public:
    decoder_iterator(std::shared_ptr<std::istream> in, uint32_t offset = 0)
    {
        __bin_size_of_current = 0;
        __raw_source = in;
        
        __raw_source->seekg(offset);
    }
    
    std::pair<K, V> current() const
    {
        return __current_data;
    }
    
    uint32_t current_position() const
    {
        return __raw_source->tellg();
    }
    
    std::pair<K, V> next()
    {
        __buffer.shift(__bin_size_of_current);
        while(__buffer.size() < sizeof(first_head_field) + sizeof(second_head_field))
        {
            if(__raw_source->eof())
                throw std::runtime_error("end of file reached");
            
            char local_buffer[SINGLE_PASS_SIZE];
            __raw_source->read(local_buffer, SINGLE_PASS_SIZE);
            if(__raw_source->fail() && !__raw_source->eof())
                throw std::runtime_error("read failure");
            __buffer.append_copy(local_buffer, __raw_source->gcount());
        }
        
        second_head_field size_header;
        const char* reader = __buffer.begin_raw() + sizeof(first_head_field);
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
        __unpack(value, value_att_header.size, decoded_pair.second);    
        
        return decoded_pair;
    }
    
    bool has_next() const
    {
        return !__raw_source->eof();
    }
    
private:
    static const int SINGLE_PASS_SIZE = 512;
    
    lsl::span __buffer;
    std::shared_ptr<std::istream> __raw_source;
    
    std::size_t __bin_size_of_current;
    std::pair<K, V> __current_data;
    
    template<typename T>
    void __unpack(const char* ptr, std::size_t size, T& data_out)
    {
        std::memcpy(&data_out, ptr, size);
    }
    void __unpack(const char* ptr, std::size_t size, std::string& data_out)
    {
        data_out.assign(ptr, size);
    }

};

}


#endif /* DECODER_ITERATOR_H */

