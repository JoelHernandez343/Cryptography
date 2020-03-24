/*
 * Created by Joel Hernández
 * Github: github.com/JoelHernandez343
 */

#ifndef J_OPERATION_MODE_HEADER_FUNCTIONS
#define J_OPERATION_MODE_HEADER_FUNCTIONS

#include <cstdlib>

#include "sdes.hpp"
#include "helpers.hpp"

namespace opmode {

    inline
    auto encryptCBC(std::string & input, unsigned char iv,std::vector<char> & ip, std::vector<char> & exp, std::vector<std::vector<char>> s0, std::vector<std::vector<char>> s1, std::vector<char> &p4, unsigned short key, std::vector<char> & p10, std::vector<char> & p8){
        
        std::string output = "";
        output.reserve(input.size());

        unsigned char prev = iv;

        for (int i = 0; i < input.size(); ++i){

            prev ^= input[i];
            prev =  sdes::encrypt(prev, ip, exp, s0, s1, p4, key, p10, p8);
            output += prev;

        }

        return output;

    }

    inline
    auto decryptCBC(std::string & input, unsigned char iv,std::vector<char> & ip, std::vector<char> & exp, std::vector<std::vector<char>> s0, std::vector<std::vector<char>> s1, std::vector<char> &p4, unsigned short key, std::vector<char> & p10, std::vector<char> & p8){

        std::string output = "";
        output.reserve(input.size());

        unsigned char prev = iv;

        for (int i = 0; i < input.size(); ++i){

            output += prev ^ sdes::decrypt(input[i], ip, exp, s0, s1, p4, key, p10, p8);
            prev = input[i];

        }

        return output;

    }

}

#endif