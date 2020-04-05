#include <iostream>

#include "table.hpp"

int main(void){

    short m = 0b100011011;
    int n = 8;

    short m1[4][4] = {
        {0x02, 0x03, 0x01, 0x01},
        {0x01, 0x02, 0x03, 0x01},
        {0x01, 0x01, 0x02, 0x03},
        {0x03, 0x01, 0x01, 0x02}
    };

    short m2[4][4] = {
        {0xE0, 0x63, 0x63, 0x63},
        {0x63, 0x63, 0x63, 0x63},
        {0x63, 0xE7, 0x63, 0x63},
        {0x63, 0x63, 0x63, 0x63}
    };

    short m3[4][4];

    for (int i = 0; i < 4; ++i){

        for (int j = 0; j < 4; ++j){

            short aux = 0;
            
            for (int k = 0; k < 4; ++k)
                aux ^= table::mult(m1[i][k], m2[k][j], m, n);
            
            m3[i][j] = aux & 0xFF;

        }

    }

    std::cout << "\u256d             \u256E\n";

    for (int i = 0; i < 4; ++i){

        std::cout << "\u2502 ";

        for (int j = 0; j < 4; ++j)
            std::cout << std::hex << m3[i][j] << " " << std::dec;

        std::cout << "\u2502\n";

    }

    std::cout << "\u2570             \u256F\n";
    return 0;

}