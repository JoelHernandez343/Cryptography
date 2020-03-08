/*
 * Created by Joel Hernández
 * Github: github.com/JoelHernandez343
 */

#ifndef J_SIMPLIFIED_DES_HEADER_FUNCTIONS
#define J_SIMPLIFIED_DES_HEADER_FUNCTIONS

#include <iostream>
#include <tuple>

#include "bitperm.hpp"
#include "helpers.hpp"

namespace sdes {

    // Calculate a mod n
    inline
    int mod(int a, int n){
        return a < 0 ? n - (-a) % n : a % n;
    }

    // Key Circular Left Shift n times
    inline 
    unsigned short circularLeftShift(unsigned short key, int n){

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

    // Generate k1 and k2
    inline
    auto generatekeys(unsigned short key, std::vector<char> & p10, std::vector<char> & p8){

        unsigned char k1, k2;

        key = bitperm::permute(key, 10, p10);
        key = circularLeftShift(key, 1);
        k1  = bitperm::permute(key, 10, p8);

        key = circularLeftShift(key, 2);
        k2  = bitperm::permute(key, 10, p8);

        return std::tuple(k1, k2);

    }

    // sdes round
    inline
    auto round(unsigned char input, std::vector<char> & exp, unsigned char key, char s0[4][4], char s1[4][4], std::vector<char> & p4){

        auto e = key ^ bitperm::permute(input, 4, exp);
        

        int i = (e & 0x80) >> 6 | (e & 0x10) >> 4;
        int j = (e & 0x40) >> 5 | (e & 0x20) >> 5;
        
        int x = (e & 0x08) >> 2 | (e & 0x01);
        int y = (e & 0x04) >> 1 | (e & 0x02) >> 1;

        auto aux = s0[i][j] << 2 | s1[x][y];

        aux = bitperm::permute(aux, 4, p4);
        aux &= 0xF;
        
        aux ^= input >> 4;

        return aux << 4 | input & 0xF;

    }

    // Encrypt / Decrypt
    inline 
    auto crypt(unsigned char input, std::vector<char> & ip, std::vector<char> & exp, char s0[4][4], char s1[4][4], std::vector<char> &p4, unsigned short key, std::vector<char> & p10, std::vector<char> & p8, bool encrypt){

        auto [k1, k2] = generatekeys(key, p10, p8);
        auto iip = bitperm::inversePermutation(ip);

        input = bitperm::permute(input, 8, ip);

        input = round(input, exp, encrypt ? k1 : k2, s0, s1, p4);
        input = ((input << 4) & 0xF0) | ((input >> 4) & 0x0F);
        input = round(input, exp, encrypt ? k2 : k1, s0, s1, p4);
        
        return bitperm::permute(input, 8, iip);

    }

    // Encrypt using sdes
    inline
    auto encrypt(unsigned char input, std::vector<char> & ip, std::vector<char> & exp, char s0[4][4], char s1[4][4], std::vector<char> &p4, unsigned short key, std::vector<char> & p10, std::vector<char> & p8){

        return crypt(input, ip, exp, s0, s1, p4, key, p10, p8, true);

    }

    // Decrypt using sdes
    inline
    auto decrypt(unsigned char input, std::vector<char> & ip, std::vector<char> & exp, char s0[4][4], char s1[4][4], std::vector<char> &p4, unsigned short key, std::vector<char> & p10, std::vector<char> & p8){

        return crypt(input, ip, exp, s0, s1, p4, key, p10, p8, false);

    }

}

#endif