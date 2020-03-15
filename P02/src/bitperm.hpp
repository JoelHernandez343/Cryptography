/*
 * Created by Joel Hernández
 * Github: github.com/JoelHernandez343
 */

#ifndef J_PERMUTATION_HEADER_FUNCTIONS
#define J_PERMUTATION_HEADER_FUNCTIONS

#include <algorithm>
#include <iostream>
#include <vector>

namespace bitperm {

    // Permute the unsigned primitive c's bits with the permutation perm and return it
    unsigned int permute(unsigned int n, int used, std::vector<char> & perm){

        unsigned int result = 0;
        short current;

        for (int i = 0; i < perm.size(); ++i){

            current = used - 1 - perm[i];

            result <<= 1;

            if (n & (1 << current))
                result |= 1;
            
        }

        return result;

    }

    // Checks if the given permutation is complete (no compression or expansion)
    inline
    bool isValidPermutation(std::vector<char> permutation){

        std::sort(permutation.begin(), permutation.end());

        for (int i = 0; i < permutation.size(); ++i)
            if (permutation[i] != i)
                return false;
        
        return true;
        
    }

    // Calculate the inverse permutation and return it
    inline
    auto inversePermutation(std::vector<char> permutation){

        if (!isValidPermutation(permutation))
            throw std::runtime_error("Invalid permutation: No inverse");

        std::vector<char> r (permutation.size());
        for (int i = 0; i < r.size(); ++i)
            r[permutation[i]] = i;

        return r;

    }

    // Generate a random permutation of size n.
    // In order to work properly, is needed init the seed of srand(), for example, with time(0); 
    inline
    auto generatePermutation(int n){

        std::vector<char> permutation(n);

        for (int i = 0; i < n; ++i)
            permutation[i] = i;

        std::random_shuffle(permutation.begin(), permutation.end());

        return permutation; 

    }

}

#endif