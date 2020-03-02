#ifndef J_SIMPLIFIED_DES_HEADER_FUNCTIONS
#define J_SIMPLIFIED_DES_HEADER_FUNCTIONS

#include <type_traits>
#include <typeinfo>
#include <iostream>
#include <vector>
#include <tuple>

#include "permutation.hpp"

namespace sdes {
    inline 
    int mod(int a, int b){
        return a < 0 ? b - (-a) % b : a % b;
    }

    // Check if the argument is a C++ primitivbe type
    template<class T>
    inline
    bool isPrimitive(const T& d){
        return std::is_fundamental<T>::value;
    }

    // Create a vector with the n first bits of the given primitive num. 
    // First element of the vector: MSB
    // Last element of the vector:  LSB
    // Supports until 64 bits
    template<class T>
    inline
    auto primitiveToVector(const T& num, int n){

        if (!isPrimitive(num)){
            std::string className (typeid(num).name());
            throw std::runtime_error("Num is not a primitive type: num is " + className);
        }

        unsigned long long aux = (unsigned long long)num;

        std::vector<bool> r(n);

        for (int i = 0; i < n; ++i)
            r[i] = (aux >> (n - i - 1)) & 1;

        return r; 

    }

    // Left shift n times
    inline
    void leftShift(std::vector<bool> & key, int n){

        if (key.size() != 10)
            throw std::runtime_error("Key size not valid.");

        std::vector<bool> aux (key);

        for (int i = 0; i < 5; ++i){
            int j = mod(i - n, 5);
            key[j] = aux[i];
            key[j + 5] = aux[i + 5];    
        }
        
    }

    // Generate k1 and k2 from a `short` key
    inline
    auto generateKeys(short key, std::vector<int> & p10, std::vector<int> & p8){

        if (p10.size() != 10 || !perm::isValidPermutation(p10))
            throw std::runtime_error("Invalid permutation p10");
        
        if (p8.size() != 8)
            throw std::runtime_error("Size of p8 permutation no valid: " + p8.size());

        auto newKey = primitiveToVector(key, 10);
        auto pKey = perm::permute(newKey, p10);
        leftShift(pKey, 1);
        auto key1 = perm::permute(pKey, p8);
        leftShift(pKey, 2);
        auto key2 = perm::permute(pKey, p8);

        return std::tuple(key1, key2);
        
    }

    // XOR operation between two vectors<bool>
    inline
    auto xorOp(std::vector<bool> const & a, std::vector<bool> const & b){

        if (a.size() != b.size() || a.size() == 0)
            throw std::runtime_error("Cannot perform xor opertation, lengths are different.");
        
        std::vector<bool> r (a.size());
        for (int i = 0; i < r.size(); ++i)
            r[i] = !(a[i] == b[i]);

        return r;

    }

    // sdes round
    inline
    auto round(std::vector<bool> & l, std::vector<bool> & r, std::vector<int> & exp, std::vector<bool> & key, bool s0[4][4][2], bool s1[4][4][2], std::vector<int> & p4){
        
        if (l.size() != 4 || r.size() != 4) 
            throw std::runtime_error("Message length invalid.");
        
        if (exp.size() != 8)
            throw std::runtime_error("Expansion permutation size invalid.");
        
        if (key.size() != 8)
            throw std::runtime_error("Key length invalid.");
    
        if (p4.size() != 4)
            throw std::runtime_error("Compression permutation size invalid.");
        
        auto expansion = perm::permute(r, exp);
        expansion = xorOp(expansion, key);
        std::vector<bool> aux4 (4);

        int i = (expansion[0] << 1) + expansion[3];
        int j = (expansion[1] << 1) + expansion[2];
        int x = (expansion[4] << 1) + expansion[7];
        int y = (expansion[5] << 1) + expansion[6];

        aux4[0] = s0[i][j][0];
        aux4[1] = s0[i][j][1];

        aux4[2] = s1[x][y][0];
        aux4[3] = s1[x][y][1];

        aux4 = perm::permute(aux4, p4);
        aux4 = xorOp(aux4, l);

        return std::tuple(aux4, r);

    }

    // Encrypt - Decrypt
    inline
    auto crypt(unsigned char input, std::vector<int> & ip,std::vector<int> & exp, bool s0[4][4][2], bool s1[4][4][2], std::vector<int> & p4, short key, std::vector<int> & p10, std::vector<int> & p8, bool encrypt){

        auto [k1, k2] = generateKeys(key, p10, p8);
        auto in = primitiveToVector(input, 8);
        auto inverseIp = perm::inversePermutation(ip);

        in = perm::permute(in, ip);

        std::vector<bool> l1 (4), r1 (4);
        for (int i = 0; i < 4; ++i){
            l1[i] = in[i];
            r1[i] = in[i + 4];
        }

        auto [l2, r2] = round(l1, r1, exp, encrypt ? k1 : k2, s0, s1, p4);
        auto [l3, r3] = round(r2, l2, exp, encrypt ? k2 : k1, s0, s1, p4);

        for (int i = 0; i < 4; ++i){
            in[i] = l3[i];
            in[i + 4] = r3[i];
        }

        return perm::permute(in, inverseIp);

    }

    // Encrypt using sdes
    inline
    auto encrypt(unsigned char message, std::vector<int> & ip,std::vector<int> & exp, bool s0[4][4][2], bool s1[4][4][2], std::vector<int> & p4, short key, std::vector<int> & p10, std::vector<int> & p8){

        return crypt(message, ip, exp, s0, s1, p4, key, p10, p8, true);

    }

    // Decrypt using sdes
    inline 
    auto decrypt(unsigned char code, std::vector<int> & ip,std::vector<int> & exp, bool s0[4][4][2], bool s1[4][4][2], std::vector<int> & p4, short key, std::vector<int> & p10, std::vector<int> & p8){

        return crypt(code, ip, exp, s0, s1, p4, key, p10, p8, false);

    }

}

#endif