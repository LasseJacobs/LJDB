
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
        __raw_source = in;
        __offset = offset;
        
        __raw_source->clear();
        __raw_source->seekg (offset, std::ios::beg);
    }
    
    std::pair<K, V> current() const
    {
        return __current_data;
    }
    
    uint32_t current_position() const
    {
        return __offset;
    }
    
    std::pair<K, V> next()
    {
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
        
        std::pair<K, V> __current_data;
        __unpack(key, key_att_header.size, __current_data.first);
        __unpack(value, value_att_header.size, __current_data.second);    
        
        __offset += (std::size_t)size_header.total_size;
        __buffer.shift((std::size_t)size_header.total_size);
        return __current_data;
    }
    
    bool has_next() const
    {
        return !__raw_source->eof() || __buffer.size() > 0;
    }
    
private:
    static const int SINGLE_PASS_SIZE = 512;
    
    lsl::span __buffer;
    std::shared_ptr<std::istream> __raw_source;
    
    uint32_t __offset;
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

