
/* 
 * File:   cli_client.cpp
 * Author: lasse
 * 
 * Created on August 16, 2019, 4:47 PM
 */

#include <iostream>

#include "cli_client.h"
#include "command_parser.h"
#include "interpreter.h"

namespace frontend {


    cli_client::cli_client(const std::string& path) : __database(path)
    {
    }

    void cli_client::run() 
    {
        eval::tokenizer tokenizer;
        eval::command_parser parser;
        eval::interpreter interpreter(std::cout, std::cerr);
        
        std::map<std::string, std::string> context_map;
        lsl::span input_buffer;
        while(true)
        {
            auto current_database_name = context_map.find("database");
            if(current_database_name != context_map.end())
                std::cout << '[' << current_database_name->second << ']' << ' ';
            
            std::cout << "> ";
            std::cin >> input_buffer;
            auto tokens = tokenizer.tokenize(eval::text_iterator(input_buffer.begin(), input_buffer.end()-1));
            
            auto stmt = parser.parse(tokens, context_map);
            try
            {
                interpreter.eval(stmt, __database, context_map);
            }
            catch(const std::exception& e)
            {
                std::cout << "an exception occurred...\n";
                std::cout << "CAUSE: " << e.what() << std::endl;
                return;
            }
            
            input_buffer.clear();
        }
    }


}

