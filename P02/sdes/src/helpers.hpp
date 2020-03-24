/*
 * Created by Joel Hernández
 * Github: github.com/JoelHernandez343
 */

#ifndef J_HELPERS_HEADER_FUNCTIONS
#define J_HELPERS_HEADER_FUNCTIONS

#include <iostream>

namespace help {

    // Prints the last n bits of c,
    // i.e., beginning with the LSB of c.
    inline
    void printBits(unsigned int c, int n){

        for (int i = n - 1; ~i; --i)
            std::cout << ((c >> i) & 1) << " ";

        std::cout << "\n";

    }

    inline
    unsigned int stringToBinary(std::string in){

        unsigned int r = 0;
        
        for (int i = 0; i < in.size(); ++i){

            r <<= 1;
            r |= in[i] != '0' ? 1 : 0;

        }

        return r;

    }

    // Generate an array of length `length` fill w/ random bits
    inline
    unsigned int randomBits(int length){
        
        unsigned int r = 0;

        for (int i = 0; i < length; ++i){
            
            r <<= 1;
            r |= rand() % 2;

        }

        return r;

    }


}

#endif