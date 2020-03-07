#include <iostream>
#include <vector>

#include "bitperm.hpp"
#include "sdes.hpp"

void printBits(unsigned short c){

    for (int i = 15; ~i; --i)
        std::cout << ((c >> i) & 1) << " ";

    std::cout << "\n";

}

int main(void){

    std::vector<char> p1 = {2, 4, 1, 6, 3, 9, 0, 8, 7, 5};
    unsigned short k = 0b1010000010;

    bitperm::permutation(k, p1);
    
    sdes::kcls(k, 1);

    printBits(k);

    return 0;

}