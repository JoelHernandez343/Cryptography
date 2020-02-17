/*
 * Created by Joel Hernández
 * Github: github.com/JoelHernandez343
 */

#ifndef AFFINE_FUNCTIONS_HEADER
#define AFFINE_FUNCTIONS_HEADER

#include <algorithm>
#include <iostream>
#include <vector>
#include <tuple>

#include "../../include/utf8.hpp"

namespace affine {

    // Module function with negatives
    inline
    int mod (int a, int n){
        return a < 0 ? n - (-a) % n : a % n;
    }

    // Show an error stylishly
    void error(std::string message){

        std::cout << "\033[40;31mERR!\033[0m " << message << "\n";
        exit(2);

    }

    // Check if the given `str` belongs to the given `alphabet`. 
    // If not, returns a string with error information, otherwise, a void string
    inline
    std::string belongsTo(std::string& str, std::string& alphabet, bool spaces){
        auto aux = utf8::trim(str);
        auto alph = utf8::trim(alphabet);

        for (int i = 0; i < aux.size(); ++i){

            if (!spaces && aux[i] == " ")
                continue;

            if (std::find(alph.begin(), alph.end(), aux[i]) == alph.end()) {

                std::string r = "\033[40;31m" + aux[i] + "\033[0m";
                for (int j = i - 1, k = 0; j >= 0 && k < 5; --j, ++k)
                    r = aux[j] + r;
                for (int j = i + 1, k = 0; j < aux.size() && k < 5; ++j, ++k)
                    r += aux[j];
                
                return r;
            }
        }

        return "";
    }

    // Return the index of the given character
    inline
    int indexOf(std::string c, std::vector<std::string>& alphabet){
        for (int i = 0; i < alphabet.size(); ++i){
            if (c == alphabet[i]) return i;
        }

        return -1;
    }

    // Calculate the modular inverse of b mod a
    // Return -1 if a, b aren't coprimes.
    // O(log(min(a, b)))
    inline
    int extEuclideanAlg(int a, int b){
        // int s = 0, s0 = 1;
        int t = 1, t0 = 0;
        int r = b, r0 = a;
        int aux, q = 0;


        while (r) {
            q = r0 / r; 

            aux = r;
            r = r0 - q * r;
            r0 = aux;

            // aux = s;
            // s = s0 - q * s;
            // s0 = s;

            aux = t;
            t = t0 - q * t;
            t0 = aux;
        }

        // Bezeout coefficients: s, t | a*s + b*t = gcd(a,b)
        // Thus, t0 is the b^-1 mod a if gcd(a,b) = 1
        // Source and explanation: https://es.wikipedia.org/wiki/Algoritmo_de_Euclides#Algoritmo_de_Euclides_extendido

        return r0 != 1 ? -1 : mod(t0, a);
    }

    // Check if a key is valid
    inline
    bool isValidKey(int k, int n){
        return extEuclideanAlg(n, k) < 0 ? false : true; 
    }

    // Generate a random key
    inline
    auto ranKey(unsigned int n){
        std::vector<int> coprimes;

        for (int i = 1; i < n; ++i){
            if (int val = extEuclideanAlg(n, i); val > 0)
                coprimes.push_back(i);
        }

        for (int i = 0; i < coprimes.size(); ++i)
            std::cout << coprimes[i] << std::endl;

        int ranIndex = rand() % coprimes.size();
        int ranB = rand() % (n - 1) + 1;

        return std::tuple(coprimes[ranIndex], ranB);
    }

    // Encrypt the given file using the alphabet and the key provided
    inline
    std::string encrypt(std::string& message, std::string& alphabet, std::tuple<int, int> key, bool spaces){
        if (std::string s; (s = belongsTo(message, alphabet, spaces)) != ""){
            error("The message provided doesn't belong to given alphabet's Kleene closure: \"" + s + "\"");
        }

        std::string r = "";
        r.reserve(message.size());

        auto msg = utf8::trim(message);
        auto alph = utf8::trim(alphabet);
        auto [a, b] = key;

        if (!isValidKey(a, alph.size())){
            std::string m = "Invalid key, the a isn't coprime of the alphabet size: gcd(";
            m += std::to_string(a);
            m.append(", ");
            m += std::to_string(alph.size());
            m.append(") != 1");
            error(m);
        }

        for (int i = 0; i < msg.size(); ++i){
            if (!spaces && msg[i] == " "){
                r += " ";
                continue;
            }

            r += alph[(a * indexOf(msg[i], alph) + b) % alph.size()];
        }

        return r;
    }

    // decrypt the given file using the alphabet and the key provided
    inline
    std::string decrypt(std::string& message, std::string& alphabet, std::tuple<int, int> key, bool spaces){
        if (std::string s; (s = belongsTo(message, alphabet, spaces)) != ""){
            error("The message provided doesn't belong to given alphabet's Kleene closure: \"" + s + "\"");
        }

        std::string r = "";
        r.reserve(message.size());

        auto msg = utf8::trim(message);
        auto alph = utf8::trim(alphabet);
        auto [a, b] = key;
        auto inverseA = extEuclideanAlg(alph.size(), a);

        for (int i = 0; i < msg.size(); ++i){
            if (!spaces && msg[i] == " "){
                r += " ";
                continue;
            }

            r += alph[mod(inverseA * (indexOf(msg[i], alph) - b), alph.size())];
        }

        return r;
    }

}

#endif