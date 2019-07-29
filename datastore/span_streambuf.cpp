
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
        setg(buf.begin(), buf.begin(), buf.begin() + buf.size());
    }

    int span_streambuf::underflow() 
    {
        return std::char_traits<char>::eof();
    }

}