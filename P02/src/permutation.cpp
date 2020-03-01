#include <iostream>
#include <ctime>

#include "permutation.hpp"

int main(void){
    std::srand(std::time(NULL));

    auto permutation = perm::generatePermutation(8);
    auto inverse     = perm::inversePermutation(permutation);

    for (auto p : permutation)
        std::cout << p << std::endl;

    std::cout << "\n";

    for (auto p : inverse)
        std::cout << p << std::endl;

    return 0;
}