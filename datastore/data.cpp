
/* 
 * File:   data.cpp
 * Author: lasse
 * 
 * Created on July 26, 2019, 4:27 PM
 */

#include "data.h"

namespace data 
{

template<typename T>
int convert_numeral_to_enum(T type)
{
    if(std::is_integral<T>::value)
        return (std::is_signed<T>::value) ? 0 : 1;
    else if (std::is_floating_point<T>::value)
        return 2;
    else
        throw std::invalid_argument("unknown type");
}

}
