/*
 * Created by Joel Hernández
 * Github: github.com/JoelHernandez343
 */

#ifndef VIGENERE_FUNCTIONS_HEADER
#define VIGENERE_FUNCTIONS_HEADER

#include <algorithm>
#include <iostream>
#include <vector>

#include "../../include/utf8.hpp"

namespace vigenere {

    // Module function with negatives
    int mod(int a, int b){
        return a < 0 ? b - ((-a) % b) : a % b;
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

    // Return a vector with the indexes of the string's chars
    inline
    std::vector<int> keyIndexes(std::vector<std::string> key, std::vector<std::string>& alphabet){

        std::vector<int> r(key.size());
        for (int i = 0; i < key.size(); ++i){
            r[i] = indexOf(key[i], alphabet);
        }

        return r;
    } 

    // Encrypt the given file using the alphabet and the key provided
    inline
    std::string encrypt(std::string& message, std::string& alphabet, std::string key, bool spaces){
        std::string s;
        
        if ((s = belongsTo(message, alphabet, spaces)) != ""){
            error("The message provided doesn't belong to given alphabet's Kleene closure: \"" + s + "\"");
        }

        if ((s = belongsTo(key, alphabet, true)) != ""){
            error("The key provided doesn't belong to given alphabet's Kleene closure: " + s);
        }

        auto msg = utf8::trim(message);
        auto alph = utf8::trim(alphabet);
        auto k = keyIndexes(utf8::trim(key), alph);

        std::string r = "";
        r.reserve(message.size());

        int j = 0;

        for (int i = 0; i < msg.size(); ++i){
            if (!spaces && msg[i] == " "){
                r += " ";
                continue;
            }

            r += alph[(indexOf(msg[i], alph) + k[j++]) % alph.size()];
            j %= k.size();
        }

        return r;
    }

    // Decrypt the given file using the alphabet and the key provided
    inline
    std::string decrypt(std::string& message, std::string& alphabet, std::string key, bool spaces){
        std::string s;
        
        if ((s = belongsTo(message, alphabet, spaces)) != ""){
            error("The message provided doesn't belong to given alphabet's Kleene closure: \"" + s + "\"");
        }

        if ((s = belongsTo(key, alphabet, true)) != ""){
            error("The key provided doesn't belong to given alphabet's Kleene closure: " + s);
        }

        auto msg = utf8::trim(message);
        auto alph = utf8::trim(alphabet);
        auto k = keyIndexes(utf8::trim(key), alph);

        std::string r = "";
        r.reserve(message.size());

        int j = 0;

        for (int i = 0; i < msg.size(); ++i){
            if (!spaces && msg[i] == " "){
                r += " ";
                continue;
            }

            r += alph[mod((indexOf(msg[i], alph) - k[j++]), alph.size())];
            j %= k.size();
        }

        return r;
    }
}



#endif