
/* 
 * File:   span.cpp
 * Author: lasse
 * 
 * Created on July 20, 2019, 5:41 PM
 */

#include "span.h"

#include <stdexcept>


#include <assert.h>
#include <iostream>
#include <algorithm>

namespace lsl
{
    const std::size_t span::DEFAULT_CAPACITY = 16;
    const float span::GROWTH_FACTOR = 1.5f;
    const float span::SHRINK_THRESHOLD = 0.1f;
    
    span::span() 
    {
        ___freeable_base_memory = new char[DEFAULT_CAPACITY];
        __memory_start = ___freeable_base_memory;
        __head_offset = 0;
        __allocated_capacity = DEFAULT_CAPACITY;
    }

    span::~span() 
    {
        delete[] ___freeable_base_memory;
    }
    
    span::span(const span& other)
    {
        ___freeable_base_memory = new char[other.__allocated_capacity];
        __memory_start = ___freeable_base_memory;
        __head_offset = other.__head_offset;
        __allocated_capacity = other.__allocated_capacity;
        
        std::memcpy(__memory_start, other.__memory_start, other.size());
    }
    
    span::span(span&& other)
    {
        *this = std::move(other);
    }
    
    span& span::operator=(const span& other)
    {
        if (this != &other) 
        {
            clear();
            append_copy(other.begin(), other.size());
        }
        return *this;
    }
    
    span& span::operator=(span&& other)
    {
        if (this != &other) 
        {
            delete[] ___freeable_base_memory;
            
            ___freeable_base_memory = other.___freeable_base_memory;
            __memory_start = other.__memory_start;
            __head_offset = other.__head_offset;
            __allocated_capacity = other.__allocated_capacity;
            
            other.___freeable_base_memory = nullptr;
            other.__memory_start = nullptr;
            other.__head_offset = 0;
            other.__allocated_capacity = 0;
        }
        return *this;
    }
    
    void span::append_copy(const void* source, std::size_t count)
    {
        __mem_copy(__head_offset, (char*)source, count);
    }
    
    void span::insert_copy(uint64_t index, const void* source, std::size_t count)
    {        
        __mem_copy(index, (const char*)source, count);
    }
    
    void span::insert_copy(char* pointer, const void* source, std::size_t count)
    {
        if(pointer < __memory_start || pointer >= (__memory_start + __allocated_capacity))
            throw std::range_error("location out of range");
        
        uint64_t offset = pointer - __memory_start;
        __mem_copy(offset, (const char*)source, count);
    }
    
    void span::skip(std::size_t count)
    {
        __head_offset += count;
    }
    
    void span::shift(std::size_t count)
    {
        //bound check
        if(count >= __allocated_capacity)
            throw std::range_error("location out of range");
        
        __memory_start += count;
        __allocated_capacity -= count;
        __head_offset = std::max(__head_offset - count, 0UL);
    }
    
    void span::clear() 
    {
        clear(__memory_start);
    }
    
    void span::clear(void* head) 
    {
        char* new_head = (char*)head;
        if(new_head < __memory_start || new_head > __memory_start + __head_offset)
            throw std::range_error("head out of range");
        
        __head_offset = new_head - __memory_start;
        //TODO: shrink if needed
    }

    char* span::begin()
    {
        return __memory_start;
    }
    
    const char* span::begin() const
    {
        return __memory_start;
    }
    
    std::size_t span::size() const
    {
        return __head_offset;
    }
    
    void span::__mem_copy(uint64_t offset, const char* source, std::size_t count)
    {
        if(__is_reallocation_needed(offset, count))
            __reallocate_for_atleast(offset, count);
        
        if((offset + count) > __head_offset)
            __head_offset = (offset + count);
        
        char* destination = __memory_start + offset;
        std::memcpy(destination, source, count);
    }
    
    bool span::__is_reallocation_needed(uint64_t offset, std::size_t count) const
    {
        return (offset > __allocated_capacity) || (__allocated_capacity - offset) < count;
    }
   
    void span::__reallocate_for_atleast(uint64_t offset, std::size_t count)
    {
        std::size_t standard_size = __allocated_capacity * GROWTH_FACTOR;
        std::size_t count_aware_size = (__allocated_capacity + count) * GROWTH_FACTOR;
        std::size_t out_of_range_size = (offset + count) * GROWTH_FACTOR;
        
        std::size_t new_capacity = std::max(out_of_range_size, std::max(standard_size, count_aware_size));
        __reallocate(new_capacity);
    }
    
    void span::__reallocate(std::size_t capacity)
    {
        //to support both shrinking and increasing of capacity
        std::size_t old_offset = std::min(__head_offset, capacity);
        
        char* new_memory = new char[capacity];
        std::memcpy(new_memory, __memory_start, old_offset);
        
        delete[] ___freeable_base_memory;
        
       __head_offset = old_offset;
       ___freeable_base_memory = new_memory;
       __memory_start = new_memory;
       __allocated_capacity = capacity;       
    }

}
