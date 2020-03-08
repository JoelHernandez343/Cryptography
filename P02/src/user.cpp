#include <iostream>
#include <vector>

#include "bitperm.hpp"
#include "sdes.hpp"

#include "helpers.hpp"

int main(void){


    std::vector<char> p10 = {2, 4, 1, 6, 3, 9, 0, 8, 7, 5};
    std::vector<char> p8  = {5, 2, 6, 3, 7, 4, 9, 8};

    unsigned short k = 0b1010000010, k1, k2;

    k = bitperm::permute(k, 10, p10);
    k = sdes::kcls(k, 1);
    k1 = bitperm::permute(k, 10, p8);

    help::printBits(k1, 8);

    k = sdes::kcls(k, 2);

    k2 = bitperm::permute(k, 10, p8);

    help::printBits(k2, 8);


    return 0;

}