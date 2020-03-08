#include <iostream>
#include <vector>

#include "bitperm.hpp"
#include "sdes.hpp"

#include "helpers.hpp"

int main(void){

    unsigned char m = 0b10111101;
    unsigned short k = 0b1010000010;

    std::vector<char> ip = {1, 5, 2, 0, 3, 7, 4, 6};
    std::vector<char> exp = {3, 0, 1, 2, 1, 2, 3, 0};

    char s0[4][4] = {
        {1, 0, 3, 2},
        {3, 2, 1, 0},
        {0, 2, 1, 3},
        {3, 1, 3, 2}
    };

    char s1[4][4] = {
        {0, 1, 2, 3},
        {2, 0, 1, 3},
        {3, 0, 1, 0},
        {2, 1, 0, 3}
    };

    std::vector<char> p4  = {1, 3, 2, 0};
    std::vector<char> p10 = {2, 4, 1, 6, 3, 9, 0, 8, 7, 5};
    std::vector<char> p8  = {5, 2, 6, 3, 7, 4, 9, 8};

    auto c = sdes::encrypt(m, ip, exp, s0, s1, p4, k, p10, p8);

    help::printBits(m, 8);

    help::printBits(c, 8);

    auto mm = sdes::decrypt(c, ip, exp, s0, s1, p4, k, p10, p8);

    help::printBits(mm, 8);

    return 0;

}