
/* 
 * File:   span.h
 * Author: lasse
 *
 * Created on July 20, 2019, 5:41 PM
 */

#ifndef SPAN_H
#define SPAN_H

#include <cstring>
#include <stdlib.h>

namespace lsl 
{

class span {
public:
    span();
    span(std::size_t size);
    
    span(const span& other) = delete;
    span& operator=(const span& other) = delete;
    
    void copy(const void* source, std::size_t count);
    void clear();
    void clear(void* head);
    
    char* begin();
    const char* begin() const;
    std::size_t size() const;
    
    virtual ~span();
private:
    static const std::size_t DEFAULT_CAPACITY;
    static const float GROWTH_FACTOR;
    static const float SHRINK_THRESHOLD;
    
    char* __memory;
    std::size_t __allocated_capacity;
    
    char* __head_pointer;
    
    std::size_t __remaining_capacity() const;
    void __reallocate(u_int32_t capacity);
};

}

#endif /* SPAN_H */

