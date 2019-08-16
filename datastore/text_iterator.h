
/* 
 * File:   text_iterator.h
 * Author: lasse
 *
 * Created on August 11, 2019, 10:24 AM
 */

#ifndef TEXT_ITERATOR_H
#define TEXT_ITERATOR_H

#include <string>
#include "span.h"


namespace eval {

class text_iterator {
public:
    text_iterator(lsl::span::const_iterator begin, lsl::span::const_iterator end);
    
    bool has_next() const;
    std::string next_word();
    lsl::span::const_iterator current_pos() const;

private:
    class delimiter {
    public:
        static const delimiter& SPACE;
        static const delimiter& PARENTHESIS;
        
        operator char() const;
        
    private:
        delimiter(char delimiter);
        
        const char __delim;
    };
    
    lsl::span::const_iterator __stream_it;
    lsl::span::const_iterator __end;
    
    std::string __next_word(lsl::span::const_iterator& stream_it, const lsl::span::const_iterator& end, delimiter token) const;
};

}

#endif /* TEXT_ITERATOR_H */

