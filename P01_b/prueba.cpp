#include <iostream>
#include <vector>

int main(void){

    std::vector<int> coprimes = {1, 3, 5, 9, 11, 13, 15, 17, 19, 23, 25, 27, 29, 31, 33, 37, 39, 41, 43, 45, 47, 51, 53, 55};

    for (int i = 0; i < coprimes.size(); ++i){
        for (int j = 0; j < 56; ++j){
            int r = (coprimes[i] * 26 + j) % 56;
            int s = (coprimes[i] * 3 + j) % 56;
            int t = (coprimes[i] * 8 + j) % 56;
            if (r == 28 && s == 43 && t == 30)
                std::cout << "a: " << coprimes[i] << " , b: " << j << "\n";
        }
    }

    return 0;
}