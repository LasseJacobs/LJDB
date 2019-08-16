
/* 
 * File:   tokenizer.h
 * Author: lasse
 *
 * Created on August 15, 2019, 9:32 PM
 */

#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <queue>

#include "text_iterator.h"


namespace eval {
    
enum class token_type_t {
    CREATE,
    DELETE,
    SELECT,
    PUT, 
    GET,
    REMOVE,
    EXIT,
    TABLE,
    DATABASE,
    STRING_ARGUMENT,
    IDENTIFIER_ARGUMENT
};

struct token_t {
    token_type_t token_type;
    std::string token_value;
};

class tokenizer {
public:
    tokenizer();
    
    std::queue<token_t> tokenize(text_iterator input) const;

private:

};

}

#endif /* TOKENIZER_H */

