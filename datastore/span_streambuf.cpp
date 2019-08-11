
/* 
 * File:   span_streambuf.cpp
 * Author: lasse
 * 
 * Created on July 29, 2019, 4:43 PM
 */

#include "span_streambuf.h"

namespace lsl
{

    span_streambuf::span_streambuf(span& buf)
    {
        setg(buf.begin_raw(), buf.begin_raw(), buf.begin_raw() + buf.size());
    }

    int span_streambuf::underflow() 
    {
        /*
        if (gptr() < egptr()) // buffer not exhausted
            return std::char_traits<char>::to_int_type(*gptr());
        
        return std::char_traits<char>::eof();
        */
        
        return gptr() == egptr() ?
                    std::char_traits<char>::eof() :
                    std::char_traits<char>::to_int_type(*gptr());
    }
    
    span_streambuf::pos_type span_streambuf::seekpos(pos_type sp, std::ios_base::openmode which) 
    {
        return seekoff(sp - pos_type(off_type(0)), std::ios_base::beg, which);
    }
    
    span_streambuf::pos_type span_streambuf::seekoff(off_type off,
                 std::ios_base::seekdir dir,
                 std::ios_base::openmode which) 
    {
        if (dir == std::ios_base::cur)
          gbump(off);
        else if (dir == std::ios_base::end)
          setg(eback(), egptr() + off, egptr());
        else if (dir == std::ios_base::beg)
          setg(eback(), eback() + off, egptr());
        return gptr() - eback();
    }

}