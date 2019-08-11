
/* 
 * File:   text_iterator.cpp
 * Author: lasse
 * 
 * Created on August 11, 2019, 10:24 AM
 */

#include "text_iterator.h"
#include "utils.h"
#include <sstream>

namespace interpreting {
    
    text_iterator::text_iterator(lsl::span::const_iterator begin, lsl::span::const_iterator end)
    {
        __stream_it = begin;
        __end = end;
        alg::iterate_until_not(__stream_it, __end, delimiter::SPACE);
    }
    
    bool text_iterator::has_next() const
    {
        return __stream_it != __end;
    }
    
    std::string text_iterator::next_word()
    {
        if(__stream_it == __end)
            throw std::range_error("iterator out of range");
        
        if(*__stream_it == delimiter::PARENTHESIS)
            return __next_word(++__stream_it, __end, delimiter::PARENTHESIS);
        else
            return __next_word(__stream_it, __end, delimiter::SPACE);
    }
    
    lsl::span::const_iterator text_iterator::current_pos() const
    {
        return __stream_it;
    }
    
    
    const text_iterator::delimiter& text_iterator::delimiter::SPACE = ' ';
    const text_iterator::delimiter& text_iterator::delimiter::PARENTHESIS = '"';
 
    text_iterator::delimiter::delimiter(char delimiter) : __delim(delimiter)
    {

    }
    
    text_iterator::delimiter::operator char() const
    {
        return __delim;
    }
    
    std::string text_iterator::__next_word(lsl::span::const_iterator& stream_it, const lsl::span::const_iterator& end,  
            text_iterator::delimiter token) const
    {        
        std::stringstream sstream;
        bool escaped = false;
        while (stream_it != end)
        {
            char cur = *(stream_it++);
            if(cur == token && (!escaped || token == delimiter::SPACE))
            {
                break;
            }
            else if(cur == '\\')
            {
                escaped = true;
                continue;
            }

            sstream << cur;
            escaped = false;
        }
        
        alg::iterate_until_not(stream_it, end, delimiter::SPACE);
        return sstream.str();
    }

}

