
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

namespace lsl
{
    const std::size_t span::DEFAULT_CAPACITY = 16;
    const float span::GROWTH_FACTOR = 1.5f;
    const float span::SHRINK_THRESHOLD = 0.1f;
    
    span::span() 
    {
        __memory = new char[DEFAULT_CAPACITY];
        __head_pointer = __memory;
        __allocated_capacity = DEFAULT_CAPACITY;
    }

    span::~span() 
    {
        delete[] __memory;
    }
    
    void span::copy(const void* source, std::size_t count)
    {
        if(__remaining_capacity() < count)
            __reallocate(count * GROWTH_FACTOR);
        
        std::memcpy(__head_pointer, source, count);
        __head_pointer += count;
    }
    
    void span::clear() 
    {
        clear(__memory);
    }
    
    void span::clear(void* head) 
    {
        char* new_head = (char*)head;
        if(new_head < __memory || new_head > __head_pointer)
            throw std::range_error("head out of range");
        
        __head_pointer = new_head;
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
        return __head_pointer - __memory;
    }
    
    std::size_t span::__remaining_capacity() const
    {
        return ((__memory + __allocated_capacity) - __head_pointer);
    }
    
    void span::__reallocate(u_int32_t capacity)
    {
        //NOTE: the order of these calls is very important
        char* new_capacity = new char[capacity];
        std::memcpy(new_capacity, __memory, capacity);
        
        std::size_t old_offset = size();
        delete[] __memory;
        
       __head_pointer = new_capacity + old_offset;
       __memory = new_capacity;
       __allocated_capacity = capacity;       
    }

}
