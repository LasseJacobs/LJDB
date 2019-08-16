
#ifndef LJDB_UTILS_H
#define LJDB_UTILS_H

#include <string>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>


#include <boost/format.hpp>


namespace utils {

    namespace string {
        
        template<typename T1>
        inline std::string format(const std::string& format, const T1& param1)
        {
            boost::format fmt = boost::format(format) % param1;
            return fmt.str();
        }
        
        template<typename T1, typename T2>
        inline std::string format(const std::string& format, const T1& param1, 
                const T2& param2)
        {
            boost::format fmt = boost::format(format) % param1 % param2;
            return fmt.str();
        }
      
    }
    
    namespace file {
        inline bool exists (const std::string& filename) 
        {
            std::ifstream f(filename);
            return f.good();
        }
        
        inline bool exists_dir(const std::string& dirname) 
        {
            struct stat st = {0};
            return (stat(dirname.c_str(), &st) != -1);
        }
        
        inline std::string merge_filename (const std::string& prefix, const std::string& filename) 
        {
            if (prefix != "" && prefix.back() != '/')
                return prefix + '/' + filename;
                
            return prefix + filename;
        }
    }
    
}

namespace alg {

    template<class InputIt, class T>
    inline void iterate_until_not(InputIt& first, const InputIt& last, const T& value) 
    {
        while (first != last) 
        {
            if (*first != value) 
            {
                return;
            }
            ++first;
        }
    }
}


#endif