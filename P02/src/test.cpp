#include <iostream>
#include <vector>

#include "bitperm.hpp"

#include "helpers.hpp"

int main(void){

    std::vector<char> p10 = {2, 4, 1, 6, 3, 9, 0, 8, 7, 5};
    std::vector<char> p8  = {5, 2, 6, 3, 7, 4, 9, 8};
    std::vector<char> ip  = {1, 5, 2, 0, 3, 7, 4, 6};
    std::vector<char> exp = {3, 0, 1, 2, 1, 2, 3, 0};
    auto iip = bitperm::inversePermutation(ip);
    unsigned short key = 0b1010000010;

    unsigned short kaux = 0b11101100;

    help::printBits(bitperm::permutev2(kaux, 8, iip), 8);



    return 0;

}