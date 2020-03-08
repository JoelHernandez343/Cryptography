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
    void printBits(unsigned int c, int n){

        for (int i = n - 1; ~i; --i)
            std::cout << ((c >> i) & 1) << " ";

        std::cout << "\n";

    }

}

#endif