/*
 * Created by Joel Hernández
 * Github: github.com/JoelHernandez343
 */

#ifndef J_SIMPLIFIED_DES_HEADER_FUNCTIONS
#define J_SIMPLIFIED_DES_HEADER_FUNCTIONS

#include <iostream>

namespace sdes {

    void printBits(unsigned short c){

    for (int i = 9; ~i; --i)
        std::cout << ((c >> i) & 1) << " ";

    std::cout << "\n";

}

    // Calculate a mod n
    inline
    int mod(int a, int n){
        return a < 0 ? n - (-a) % n : a % n;
    }

    // Key Circular Left Shift n times
    inline 
    void kcls(unsigned short & key, int n){

        short aux;
        while (n --> 0){

            aux =  (key & 0x0200) >> 4;
            aux |= (key & 0x0010) >> 4;

            key <<= 1;
            key |= aux;

        }

        key &= 0x02FF;

    }

}

#endif