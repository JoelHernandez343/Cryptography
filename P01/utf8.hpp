/*
 * Created by Joel Hernández
 * Github: github.com/JoelHernandez343
 * Usage: #include "utf8.hpp"
 *        utf::trim(std::string)
 */

#ifndef UFT8_FUNCTIONS_HEADER
#define UFT8_FUNCTIONS_HEADER

#include <iostream>
#include <vector>

namespace utf8 {
    
    // Trim a utf8 `string` in individual strings with its own utf8 char and return a `vector` with them.
    // The `string` must be utf8 codified. 
    inline
    std::vector< std::string > trim(std::string str){
        std::vector< std::string > trim;

        int i = 0;
        while (str[i]){
            std::string aux = "";
            int j = 0;
            
            while ((str[i] >> (7 - j)) & 1) ++j;
            
            if (j == 0) j = 1;

            for (int k = 0; k < j; ++k){
                aux += str[i++];
            }

            trim.push_back(aux);
        }

        return trim;
    }
}

#endif