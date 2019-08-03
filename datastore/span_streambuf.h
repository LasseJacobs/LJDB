
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
    pos_type seekpos(pos_type sp, std::ios_base::openmode which) override;
    pos_type seekoff(off_type off,
                 std::ios_base::seekdir dir,
                 std::ios_base::openmode which = std::ios_base::in) override;
    
};

}

#endif /* SPAN_STREAMBUF_H */

