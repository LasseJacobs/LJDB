
#ifndef LJDB_UTILS_H
#define LJDB_UTILS_H

#include <string>
#include <fstream>

#include <boost/format.hpp>


namespace utils {

    namespace string {

        inline std::string format(const std::string& format, const std::string& param1)
        {
            boost::format fmt = boost::format(format) % param1;
            return fmt.str();
        }
        
        inline std::string format(const std::string& format, const std::string& param1, 
                const std::string& param2)
        {
            boost::format fmt = boost::format(format) % param1 % param2;
            return fmt.str();
        }

    }
    
    namespace file {
        inline bool exists (const std::string& filename) {
            std::ifstream f(filename);
            return f.good();
        }
        
        inline std::string merge_filename (const std::string& prefix, const std::string& filename) {
            if (prefix != "" && prefix.back() != '/')
                return prefix + '/' + filename;
                
            return prefix + filename;
        }
    }
}


#endif