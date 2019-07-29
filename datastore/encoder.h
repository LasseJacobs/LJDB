
/* 
 * File:   encoder.h
 * Author: lasse
 *
 * Created on July 18, 2019, 11:13 AM
 * NOTE: this class is _not_ thread-safe
 */

#ifndef ENCODER_H
#define ENCODER_H

#include <utility>
#include "span.h"
#include "data.h"

namespace data 
{

class encoder {
public:
    
    template<typename K, typename V>
    blob encode(std::pair<K, V> record);
    
private:
    lsl::span __buffer;
    
    template<typename T>
    size_t __encode_attribute(T attribute); 
};

}
 
#endif /* ENCODER_H */

