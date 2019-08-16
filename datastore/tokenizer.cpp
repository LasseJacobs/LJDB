
/* 
 * File:   tokenizer.cpp
 * Author: lasse
 * 
 * Created on August 15, 2019, 9:32 PM
 */

#include "tokenizer.h"

namespace eval {

    tokenizer::tokenizer() 
    {
        
    }

    std::queue<token_t> tokenizer::tokenize(text_iterator input) const 
    {
        std::queue<token_t> token_chain;
        while(input.has_next()) 
        {
            std::string word = input.next_word();
            if(word == "create")
                token_chain.push({token_type_t::CREATE, word});
            else if(word == "delete")
                token_chain.push({token_type_t::DELETE, word});
            else if(word == "select")
                token_chain.push({token_type_t::SELECT, word});
            else if(word == "put")
                token_chain.push({token_type_t::PUT, word});
            else if(word == "get")
                token_chain.push({token_type_t::GET, word});
            else if(word == "remove")
                token_chain.push({token_type_t::REMOVE, word});
            else if(word == "exit")
                token_chain.push({token_type_t::EXIT, word});
            else if(word == "table")
                token_chain.push({token_type_t::TABLE, word});
            else if(word == "database")
                token_chain.push({token_type_t::DATABASE, word});
            else if(word[0] == '"')
                token_chain.push({token_type_t::STRING_ARGUMENT, word});
            else
                token_chain.push({token_type_t::IDENTIFIER_ARGUMENT, word});
        }
        
        return token_chain;
    }


}