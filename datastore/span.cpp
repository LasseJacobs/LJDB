
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
        __memory = new char[DEFAULT_CAPACITY];
        __head_offset = 0;
        __allocated_capacity = DEFAULT_CAPACITY;
    }

    span::~span() 
    {
        delete[] __memory;
    }
    
    span::span(const span& other)
    {
        __memory = new char[other.__allocated_capacity];
        __head_offset = other.__head_offset;
        __allocated_capacity = other.__allocated_capacity;
        
        std::memcpy(__memory, other.__memory, other.size());
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
            copy(other.begin(), other.size());
        }
        return *this;
    }
    
    span& span::operator=(span&& other)
    {
        if (this != &other) 
        {
            delete[] __memory;
            
            __memory = other.__memory;
            __head_offset = other.__head_offset;
            __allocated_capacity = other.__allocated_capacity;

            other.__memory = nullptr;
            other.__head_offset = 0;
            other.__allocated_capacity = 0;
        }
        return *this;
    }
    
    void span::copy(const void* source, std::size_t count)
    {
        __mem_copy(__head_offset, (char*)source, count);
    }
    
    void span::insert_copy(uint64_t index, const void* source, std::size_t count)
    {        
        __mem_copy(index, (const char*)source, count);
    }
    
    void span::insert_copy(char* pointer, const void* source, std::size_t count)
    {
        if(pointer < __memory || pointer >= (__memory + __allocated_capacity))
            throw std::range_error("location out of range");
        
        uint64_t offset = pointer - __memory;
        __mem_copy(offset, (const char*)source, count);
    }
    
    void span::skip(std::size_t count)
    {
        __head_offset += count;
    }
    
    void span::clear() 
    {
        clear(__memory);
    }
    
    void span::clear(void* head) 
    {
        char* new_head = (char*)head;
        if(new_head < __memory || new_head > __memory + __head_offset)
            throw std::range_error("head out of range");
        
        __head_offset = new_head - __memory;
        //TODO: shrink if needed
    }

    char* span::begin()
    {
        return __memory;
    }
    
    const char* span::begin() const
    {
        return __memory;
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
        
        char* destination = __memory + offset;
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
        
        char* new_capacity = new char[capacity];
        std::memcpy(new_capacity, __memory, old_offset);
        
        delete[] __memory;
        
       __head_offset = old_offset;
       __memory = new_capacity;
       __allocated_capacity = capacity;       
    }

}
