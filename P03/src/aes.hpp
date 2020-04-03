/**
 * Created by Joel Hernández
 * Github: https://github.com/JoelHernandez343
 */
#ifndef J_AES_HEADER_FUNCTIONS
#define J_AES_HEADER_FUNCTIONS

#include <iostream>
#include <sstream>
#include <iomanip>

namespace aes {

    unsigned char rc[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36, 0x6C, 0xD8};
    unsigned char sBox[][16]{
        {0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76},
        {0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0},
        {0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15},
        {0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75},
        {0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84},
        {0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF},
        {0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8},
        {0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2},
        {0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73},
        {0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB},
        {0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79},
        {0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08},
        {0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A},
        {0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E},
        {0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF},
        {0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16}
    };

    // char to int, get the hexadecimal value of the given char
    // return -1 if a invalid char is given
    inline
    unsigned char ctoi(char c){

        if (c >= '0' && c <= '9')
            return c - '0';
        
        if (c >= 'a' && c <= 'f')
            return c - 'a' + 10;
        
        if (c >= 'A' && c <= 'F')
            return c - 'A' + 10;
        
        return -1;

    }

    // Convert a hexadecimal key string representation to 
    // a integer vector with that values
    inline
    auto getKeyFromStr(std::string & keyString){

        std::vector<unsigned int> key (keyString.size() / 8);

        for (int i = 0; i < keyString.size(); i += 8){

            unsigned int aux = 0;

            for (int j = 0; j < 8; ++j)
                aux = aux << 4 | ctoi(keyString[i + j]);
            
            key[i >> 3] = aux;

        }
        
        return key;

    }

    // Rotate the given word
    inline
    void shiftColumn(unsigned int & column){

        auto aux = column >> 24;
        
        column = column << 8 | aux;

    }

    // Substitute the values of the column with the sbox's values
    inline
    void sBoxSubs(unsigned int & column){

        unsigned char * aux = (unsigned char *)(&column);
        
        for (int i = 0; i < 4; ++i)
            aux[i] = sBox[aux[i] >> 4][aux[i] & 0xF];

    }

    // XOR with the round coeffient
    inline
    void roundCoeffiecient(unsigned int & column, int i){

        column ^= rc[i] << 24;

    }

    // XOR with the rest of the columns
    inline
    void finalBuild(std::vector<unsigned int> & key, unsigned int column){

        for (int i = 0; i < key.size(); ++i){

            key[i] ^= column;
            column = key[i];

        }

    }

    // AES key schedule round
    inline
    void round(std::vector<unsigned int> & key, int i){

        unsigned int column = key[key.size() - 1];

        shiftColumn(column);
        sBoxSubs(column);
        roundCoeffiecient(column, i);
        finalBuild(key, column);

    }

    // The key schedule process. Receives a string with the key's hexadecimal representation
    // If the given key is 128 bits length (32 characters), executes 10 rounds
    // If the given key is 192 bits length (48 characters), executes 8 truncated rounds
    // Returns a integer vector with all words generated (44 if 128, 52 if 192).
    inline
    auto keySchedule(std::string keyString){

        if (keyString.size() != 32 && keyString.size() != 48)
            return std::vector<unsigned int>(0);

        auto key = getKeyFromStr(keyString);

        std::vector<unsigned int> result(key.size() == 4 ? 44 : 52);

        for (int i = 0; i < result.size(); i += key.size()){

            for (int j = 0; j < key.size() && i + j < result.size() ; ++j)
                result[i + j] = key[j];
            
            // There is no point in making a round that will not be occupied
            if (i + key.size() >= result.size())
                break;
                
            round(key, i / key.size());
            
        }

        return result;

    }

    // Create a string with a aes matrix
    auto aesMatrix(std::vector<unsigned int> & values){

        unsigned char tmp[4][values.size()];

        // Create the matrix with the words as columns
        for (int j = 0; j < values.size(); ++j){

            unsigned char * aux = (unsigned char *)&values[j];

            for (int i = 0; i < 4; ++i)
                tmp[i][j] = aux[3 - i];

        }

        std::stringstream buffer;

        // Copy the matrix like hexadecimals in a buffer
        for (int i = 0; i < 4; ++i){

            std::ios::fmtflags flags(buffer.flags());

            for (int j = 0; j < values.size(); ++j)
                buffer << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)tmp[i][j] << " ";

            buffer.flags(flags);
            buffer << "\n";

        }

        // Return the string inside the buffer
        return buffer.str();

    }

}

#endif