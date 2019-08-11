
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
#include <cstdint>

/*
 * TODO
 * ----------
 * [ ] Make thread-safe
 * [ ] Implement some of the common stl operations
 * [ ] Make shallow const copy operation (that allows the positioning data to change but not the underlying buffer) 
 *
 */


namespace lsl 
{

class span {
public:
    typedef char* iterator;
    typedef char const* const_iterator;
    
    span(std::size_t size = DEFAULT_CAPACITY);
    span(const char* c_str);
    span(const span& other);
    span(span&& other);
    
    span& operator=(const span& other);
    span& operator=(span&& other);
    
    void append_copy(const void* source, std::size_t count);
    void insert_copy(uint64_t index, const void* source, std::size_t count);
    void insert_copy(char* pointer, const void* source, std::size_t count);
    
    void skip(std::size_t count);
    void shift(std::size_t count);

    void clear();
    void clear(void* head);
    
    char* begin_raw();
    const char* begin_raw() const;
    std::size_t size() const;
    
    iterator begin();
    const_iterator begin() const;
    
    iterator end();
    const_iterator end() const;
    
    //Shrink to fit, pre_allocate and << operators maybe
    
    virtual ~span();
private:
    static const std::size_t DEFAULT_CAPACITY;
    static const float GROWTH_FACTOR;
    static const float SHRINK_THRESHOLD;
    
    char* ___freeable_base_memory;
    
    char* __memory_start;
    std::size_t __allocated_capacity;
    
    uint64_t __head_offset;
    
    void __mem_copy(uint64_t offset, const char* source, std::size_t count);
    
    bool __is_reallocation_needed(uint64_t offset, std::size_t count) const;
    void __reallocate_for_atleast(uint64_t offset, std::size_t count);
    void __reallocate(std::size_t capacity);
};

}

#endif /* SPAN_H */

