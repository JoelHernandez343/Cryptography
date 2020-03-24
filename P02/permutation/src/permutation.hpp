/*
 * Created by Joel Hernández
 * Github: github.com/JoelHernandez343
 */

#ifndef J_PERMUTATION_HEADER_FUNCTIONS
#define J_PERMUTATION_HEADER_FUNCTIONS

#include <algorithm>
#include <iostream>
#include <vector>

#include "../../include/utf8.hpp"

namespace perm {

    // Checks if the given permutation has inverse
    inline
    bool hasInverse(std::vector<int> permutation){

        if (!permutation.size())
            return false;

        std::sort(permutation.begin(), permutation.end());

        for (int i = 0; i < permutation.size(); ++i)
            if (permutation[i] != i)
                return false;
        
        return true;
        
    }

    // Permute the vector set with the permutation provided
    template<typename T>
    inline
    auto permute(std::vector<T> const& set, std::vector<int> permutation){

        std::vector<T> r (permutation.size());
        for (int i = 0; i < r.size(); ++i)
            r[i] = set[permutation[i]];

        return r;

    }

    // Calculate the inverse permutation
    inline
    auto inversePermutation(std::vector<int> permutation){

        if (!hasInverse(permutation))
            return std::vector<int>(0);

        std::vector<int> r (permutation.size());
        for (int i = 0; i < r.size(); ++i)
            r[permutation[i]] = i;

        return r;

    }

    // Generate a random permutation of size n
    inline
    auto generatePermutation(int n){

        std::vector<int> permutation(n);

        for (int i = 0; i < n; ++i)
            permutation[i] = i;

        std::random_shuffle(permutation.begin(), permutation.end());

        return permutation; 

    }

    // Encrypt / Decrypt the given utf8 string with the permutation.
    // Autocomplete with 0's
    inline
    auto encrypt(std::string input, std::vector<int> perm){

        auto in = utf8::trim(input);

        int d = (perm.size() - (in.size() % perm.size())) % perm.size();

        if (d)
            std::cout << "\033[40;33mWARN\033[0m Added " << d << " 0's to end\n";

        for (int i = 0; i < d; ++i)
            in.push_back("0");
        
        std::string out = "";
        out.reserve(in.size());

        for (int i = 0; i < in.size(); i += perm.size()){

            std::vector<std::string> aux(perm.size());
            for (int j = 0; j < perm.size(); ++j)
                aux[j] = in[j + i];

            aux = permute(aux, perm);

            for (int j = 0; j < perm.size(); ++j)
                out += aux[j];

        }

        return out;

    }

}

#endif