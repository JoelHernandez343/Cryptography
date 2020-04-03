/**
 * Created by Joel Hernandez
 */

#include <iostream>
#include <sstream>
#include <iomanip>

#include "table.hpp"
#include "aes.hpp"

void writeToFile(std::string s, std::string file){

    std::ofstream writer(file, std::ios::trunc);

    writer << s;
    writer.close();

}

void aesProcess(){

    std::string key;
    std::cin >> key;

    auto result = aes::keySchedule(key);
    int s = result.size() == 44 ? 4 : 6;

    std::stringstream buffer;

    for (int i = 0; i < result.size(); i += s){

        std::vector<unsigned int> aux;
        aux.reserve(s);

        for (int j = 0; j < s && i + j < result.size(); ++j)
            aux.push_back(result[i + j]);

        buffer << "Round: " << (i / s) << "\n";
        buffer << aes::aesMatrix(aux);

    }

    writeToFile(buffer.str(), "key.txt");
    std::cout << "OUTPUT: key.txt\n";

}

int main(void){

    int opt;
    std::cin >> opt;

    if (opt == 0)
        table::writeTable();
    else if (opt == 1)
        aesProcess();
    else
        std::cout << "Invalid Option";

    return 0;

}