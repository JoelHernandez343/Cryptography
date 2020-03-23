#ifndef J_PERMUTATION_HEADER_FUNCTIONS
#define J_PERMUTATION_HEADER_FUNCTIONS

#include <algorithm>
#include <iostream>
#include <vector>

namespace perm {

    // Checks if the given permutation is valid
    bool isValidPermutation(std::vector<int> permutation){

        std::sort(permutation.begin(), permutation.end());

        for (int i = 0; i < permutation.size(); ++i)
            if (permutation[i] != i)
                return false;
        
        return true;
        
    }

    // Permute the vector set with the permutation provided
    template<typename T, typename A>
    auto permutation(std::vector<T,A> const& set, std::vector<int> permutation){

        if (!isValidPermutation(permutation))
            throw std::runtime_error("Invalid permutation");

        std::vector<T, A> r (set.size());
        for (int i = 0; i < set.size(); ++i)
            r[i] = set[permutation[i]];

        return r;

    }

    // Calculate the inverse permutation
    auto inversePermutation(std::vector<int> permutation){

        if (!isValidPermutation(permutation))
            throw std::runtime_error("Invalid permutation");

        std::vector<int> r (permutation.size());
        for (int i = 0; i < r.size(); ++i)
            r[permutation[i]] = i;

        return r;

    }

    // Generate a random permutation of size n
    auto generatePermutation(int n){

        std::vector<int> permutation(n);

        for (int i = 0; i < n; ++i)
            permutation[i] = i;

        std::random_shuffle(permutation.begin(), permutation.end());

        return permutation; 

    }

}

#endif