#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <cstdint>
#include <unordered_map>
#include <vector>

#include <cryptopp/cryptlib.h>
#include <cryptopp/integer.h>
#include <cryptopp/nbtheory.h>

typedef unsigned long long ull;

// Get a decimal string representation of i without the last dot put by CryptoPP
std::string decCleanOfInteger (CryptoPP::Integer i){

    std::ostringstream oss;
    oss << i;

    std::string str(oss.str());
    str.pop_back();

    return str;

}

// Encode the Integer n to a string of chars
std::string integerToHex (CryptoPP::Integer n) {

    std::stringstream ss;
    ss << std::hex << n;
    
    auto str = std::string(ss.str());
    str.pop_back();

    return str;
}

// Get the hex value of c if valid, -1 otherwise
char getHex(char c) {

    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    
    return -1;

}

// Converts (if is possible) the Integer to unsigned long long
ull integerToULL(CryptoPP::Integer n) {

    auto aux = integerToHex(n);

    if (aux.size() > 16)
        return -1;

    ull r = 0;
    for (int i = 0; i < aux.size(); ++i){

        r <<= 4;
        r |= getHex(aux[i]) & 0x0f;

    }

    return r;

}

// Computes x such that b^x % mod == result
// Based on https://en.wikipedia.org/wiki/Baby-step_giant-step#C++_algorithm_(C++17)
ull babystep_giantstep(ull b, ull result, ull mod){

    const auto m = (ull)(std::ceil(std::sqrt(mod)));
    auto map = std::unordered_map<ull, ull>{};

    CryptoPP::Integer base = b, exp = (mod - m - 1), module = mod, e = 1;

    for (ull i = 0; i < m; ++i){

        map[integerToULL(e)] = i;
        e = a_times_b_mod_c(e, base, module);

    }

    const auto factor = a_exp_b_mod_c(base, exp, module);
    e = result;

    for (ull i = 0; i < m; ++i){

        auto it = map.find(integerToULL(e));
        if (it != map.end())
            return i * m + it->second;
        e = a_times_b_mod_c(e, factor, module);

    }

    return -1;

}

// Represents a query of the first problem
struct query {
    ull base, result, module;

    query (ull b, ull r, ull m) : base(b), result(r), module(m) {};
};

// Get prime factors of n
std::vector<ull> getPrimeFactors(ull n){

    auto r = std::vector<ull>();

    if (n % 2 == 0)
        r.push_back(2);
    while (n % 2 == 0)
        n /= 2;

    for (ull i = 3; i * i <= n; ++i){

        if (n % i == 0)
            r.push_back(i);
        while (n % i == 0)
            n /= i;

    }

    if (n > 2)
        r.push_back(n);

    return r;

}

int main(void) {

    std::cout << "Develop a program to solve the discrete logarithm problem, for the following instances:\n\n";

    query queries[] = {
        query(11, 400, 1009),
        query(5, 5235, 10007),
        query(2, 1922556950ULL, 100000000003ULL),
        query(3, 406870124ULL, 500000009ULL),
        query(3, 187776257ULL, 500000009ULL)
    };

    for (int i = 0; i < 5; ++i){
        auto b = queries[i].base, r = queries[i].result, m = queries[i].module;
        std::cout << b << "^x mod " << m << " = " << r << " => x = " << babystep_giantstep(b, r, m) << "\n";
    }

    std::cout << "\n\nDevelop a program to find the prime factors of the following composite numbers:\n\n";

    std::vector<ull> numbers = {
        100160063ULL,
        10006200817ULL,
        250035001189ULL,
        250000009000000081ULL,
    };

    for (int i = 0; i < numbers.size(); ++i) {

        std::cout << "Factors of " << numbers[i] << ":\n";

        auto primesFactors = getPrimeFactors(numbers[i]);

        for (int j = 0; j < primesFactors.size(); ++j)
            std::cout << primesFactors[j] << " ";
        std::cout << "\n";

    }
    

    return 0;

}