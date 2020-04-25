/*
 * Generate 100 pseudorandom integers of 64 bits
 */

#include <iostream>

#include <cryptopp/cryptlib.h>
#include <cryptopp/osrng.h>
#include <cryptopp/integer.h>

int main(void){

    // Create a pseudo random number generator
    CryptoPP::AutoSeededRandomPool prng;
    
    // Internally creates a Integer with the pseudorandom generator and the length
    for (int i = 0 ; i < 100; ++i)
        std::cout << i << " " << std::hex << CryptoPP::Integer(prng, 64) << std::dec << "\n";

    return 0;

}