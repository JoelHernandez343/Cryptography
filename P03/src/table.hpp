/**
 * Created by Joel Hernandez
 */

#ifndef J_TABLE_GF2_HEADER_FUNCTIONS
#define J_TABLE_GF2_HEADER_FUNCTIONS

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

namespace table {

    inline
    void printBits(int c, int n){

        for (int i = n - 1; ~i; --i)
            std::cout << ((c >> i) & 1) << " ";

    }

    inline
    std::string bits(int c, int n){

        std::stringstream r;

        for (int i = n - 1; ~i; --i)
            r << ((c >> i) & 1) << " ";

        return r.str();

    }

    inline
    void printHex(int c, int n){

        int aux = n < 4 ? ~((-1) << n) : 0xF;

        n = n / 4 + (n % 4 ? 1 : 0); 

        for (int i = n - 1; ~i; --i)
            std::cout << std::hex << ((c >> (4 * i)) & aux) << std::dec;

    }

    inline
    std::string hex(int c, int n){

        std::stringstream r;

        int aux = n < 4 ? ~((-1) << n) : 0xF;

        n = n / 4 + (n % 4 ? 1 : 0); 

        for (int i = n - 1; ~i; --i)
            r << std::hex << ((c >> (4 * i)) & aux) << std::dec;

        return r.str();

    }

    inline
    short multByX(short a, short m, int n){

        m &= ~((-1) << n);

        return (a & (1 << (n - 1))) ? ((a << 1) ^ m) : (a << 1); 

    }

    inline
    short multByManyX(short a, short m, int n, int times){

        if (times == 0) return a;
        if (times == 1) return multByX(a, m, n);

        for (int i = 0; i < times; ++i)
            a = multByX(a, m, n);
        
        return a;

    }

    inline
    short mult(short a, short b, short m, int n){

        bool flag = false;
        short result = 0;

        for (int i = 0; i < n; ++i){

            if (b & (1 << i)){

                result = !flag ? multByManyX(a, m, n, i) : result ^ multByManyX(a, m, n, i);
                flag = true;

            } 

        }

        return result;

    }

    inline
    void writeTable(){

        std::ofstream writer("table.txt", std::ios::trunc);

        int n;
        std::cin >> n;

        std::string opt;
        std::cin >> opt;

        bool bin = opt == "bin";

        std::vector<short> polynomials = {
            0, 0, // Useless
            0b1011,
            0b10011,
            0b100101,
            0b1000011,
            0b10000011,
            0b100011011
        };

        short m = polynomials.at(n - 1);

        writer << "m(x): ";
        bin ? (writer << bits(m, n + 1)) : (writer << hex(m, n + 1));
        writer << "\n";

        for (int i = 1; i < (1 << n); ++i){

            for (int j = 1; j < (1 << n); ++j){

                bin ? (writer << bits(mult(i, j, m, n), n)) : (writer << hex(mult(i, j, m, n), n));
                writer << " ";

            }

            writer << "\n";

        }

        writer.close();

        std::cout << "Output: table.txt\n";

    }

}

#endif