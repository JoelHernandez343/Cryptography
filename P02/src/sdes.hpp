/*
 * Created by Joel Hernández
 * Github: github.com/JoelHernandez343
 */

#ifndef J_SIMPLIFIED_DES_HEADER_FUNCTIONS
#define J_SIMPLIFIED_DES_HEADER_FUNCTIONS

#include <iostream>

#include "helpers.hpp"

namespace sdes {

    // Calculate a mod n
    inline
    int mod(int a, int n){
        return a < 0 ? n - (-a) % n : a % n;
    }

    // Key Circular Left Shift n times
    inline 
    unsigned short kcls(unsigned short key, int n){

        short aux;
        while (n --> 0){

            aux =  (key & 0x0200) >> 4;
            aux |= (key & 0x0010) >> 4;

            key <<= 1;
            key &= 0xFFDE;
            key |= aux;

        }

        return key &= 0x02FF;

    }

}

#endif