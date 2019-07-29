
/* 
 * File:   span_streambuf.h
 * Author: lasse
 *
 * Created on July 29, 2019, 4:43 PM
 */

#ifndef SPAN_STREAMBUF_H
#define SPAN_STREAMBUF_H

#include <sstream>

#include "span.h"

namespace lsl 
{

class span_streambuf : public std::streambuf {
    
public:
    span_streambuf(span& buf);
    
protected:
    int underflow();
    
    
};

}

#endif /* SPAN_STREAMBUF_H */

