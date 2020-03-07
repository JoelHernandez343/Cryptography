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

    // Permute the unsigned primitive c's bits with the permutation perm
    template<class T, typename = typename std::enable_if<std::is_unsigned<T>::value>::type >
    inline
    void permutation(T & c, std::vector<char> & perm){

        unsigned int tmp = -1, aux;
        char shift, current, roof = perm.size() - 1;
        bool right;

        for (int i = roof; ~i; --i){

            current = roof - perm[roof - i];

            aux = c | ~(1 << current);

            shift = i - current;
            right = shift < 0;
            shift = right ? -shift : shift;

            while (shift --> 0)
                aux = right ? (aux >> 1) | 0x80000000 : (aux << 1) | 0x01;

            tmp &= aux;

        }

        roof++;
        aux = 0;
        while (roof --> 0)
            aux |= 1 << roof;

        c = aux & (T)tmp;

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

    // Calculate the inverse permutation
    inline
    auto inversePermutation(std::vector<char> permutation){

        if (!isValidPermutation(permutation))
            throw std::runtime_error("Invalid permutation: No inverse");

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

}

#endif