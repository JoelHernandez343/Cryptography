/*
 * Created by Joel Hernández
 * Github: github.com/JoelHernandez343
 */

#include <iostream>
#include <vector>

#include "sdes.hpp"

int main(void){

    unsigned char message = 0b10111101;
    short key = 0b1010000010;

    std::vector<int> ip = {1, 5, 2, 0, 3, 7, 4, 6};
    std::vector<int> exp = {3, 0, 1, 2, 1, 2, 3, 0};

    bool s0[4][4][2] = {
        {{0, 1}, {0, 0}, {1, 1}, {1, 0}},
        {{1, 1}, {1, 0}, {0, 1}, {0, 0}},
        {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
        {{1, 1}, {0, 1}, {1, 1}, {1, 0}}
    };

    bool s1[4][4][2] = {
        {{0, 0}, {0, 1}, {1, 0}, {1, 1}},
        {{1, 0}, {0, 0}, {0, 1}, {1, 1}},
        {{1, 1}, {0, 0}, {0, 1}, {0, 0}},
        {{1, 0}, {0, 1}, {0, 0}, {1, 1}}
    };

    std::vector<int> p4 = {1, 3, 2, 0};
    std::vector<int> p10 = {2, 4, 1, 6, 3, 9, 0, 8, 7, 5};
    std::vector<int> p8  = {5, 2, 6, 3, 7, 4, 9, 8};

    auto c = sdes::encrypt(message, ip, exp, s0, s1, p4, key, p10, p8); 

    unsigned char code = 0;
    for (int i = 0; i < 8; ++i)
        code += c[i] << (7 - i);
    
    auto m = sdes::decrypt(code, ip, exp, s0, s1, p4, key, p10, p8);

    for (auto e : m)
        std::cout << e << " ";
    std::cout << "\n\n";

    return 0;
}