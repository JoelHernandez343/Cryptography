#include <iostream>
#include <time.h> // I'm Thanos
#include <cstdlib> // For rand
#include <vector>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <tuple>
#include <string>

#include <json.hpp>
#include <cxxopts.hpp>

#include "../utf8.hpp"
#include "affine.hpp"

// Show an error stylishly
void error(std::string message ,cxxopts::Options& options){

    std::cout << "\033[40;31mERR!\033[0m " << message << "\n";
    std::cout << options.help({""}) << "\n";
    exit(1);

}

// Check is an alphabet has white space
bool checkSpace(std::string& alphabet){
    auto alph = utf8::trim(alphabet);

    return std::find(alph.begin(), alph.end(), " ") == alph.end() ? false : true;
}

// Read a file and outputs a single line with the whole file
std::string readFromFile(std::string file, cxxopts::Options& options){
    if (!std::filesystem::exists(file)){
        error("Input file " + file + " doesn't exists.", options);
    }

    std::ifstream reader (file);
    std::string r = "", aux;

    while (getline(reader, aux)){
        r.append(aux);
        r.append(" ");
    }

    r.pop_back();

    return r;
}

// Write the message in the given file's path
void writeToFile(std::string& message, std::string file, bool encrypt){
    
    std::string ext = encrypt ? ".aff" : ".txt";

    int dot = file.find(".");

    if (dot == std::string::npos){
        file += ext;
    }
    else {
        std::string aux = file.substr(0, dot);
        file = aux + ext;
    }

    std::ofstream writer (file, std::ios::trunc);
    writer << message;
    writer.close();

    std::cout << "\033[40;32mDONE\033[0m Output file: " << file << "\n";
}

// Process the json file
void jsonParsing(std::string jsonFile, bool encrypt, cxxopts::Options& options){

    if (!std::filesystem::exists(jsonFile)){
        error("File " + jsonFile + " doesn't exist.", options);
    }

    std::ifstream reader(jsonFile);
    std::istreambuf_iterator<char> it(reader), end;
    std::string stringJSON(it, end);
    reader.close();

    nlohmann::json j = nlohmann::json::parse(stringJSON);

    if (!j.count("alphabet") || !j.count("key")){
        error("Missing json fields in " + jsonFile + ". Check it.", options);
    }

    if ((encrypt && !j.count("messageFile")) || (!encrypt && !j.count("encryptedMessageFile"))){
        std::string m = "You must provide a file name in order to ";
        m.append(encrypt ? "encrypt." : "decrypt.");
        error(m, options);
    }

    std::string file = encrypt ? j["messageFile"] : j["encryptedMessageFile"], alphabet = j["alphabet"];
    auto key = j["key"];

    bool spaces = j.count("spaces") && j["spaces"] ? true : false;
    bool hasSpace = checkSpace(alphabet);

    if (!spaces && hasSpace)
        spaces = true;

    if (spaces && !hasSpace){
        alphabet.append(" ");
    }

    std::tuple<int, int> keyToUse;

    if (key.count("a") && key.count("b")){
        int a = key["a"];
        int b = key["b"];

        keyToUse = std::tuple(a, b);
    }
    else if (encrypt && key.count("random") && key["random"]){
        std::cout << "\033[40;33mWARN\033[0m Using random key. Generating ...\n";
         
        auto alph = utf8::trim(alphabet);
        keyToUse = affine::ranKey(alph.size());

        auto [a, b] = keyToUse;
        std::cout << "\033[40;33mWARN\033[0m Key used is a: " << a << " and b: " << b << "\n";
    }
    else {
        error("You must provide a and b (necessary to decrypt) or indicate random true value in key field of json file.", options);
    }

    std::string input = readFromFile(file, options); 
    std::string output = encrypt ? affine::encrypt(input, alphabet, keyToUse, spaces) : affine::decrypt(input, alphabet, keyToUse, spaces);

    writeToFile(output, file, encrypt);
}

int main(int argc, char * argv[]) {
   
    std::srand(time(NULL));

    // Description of the application
    cxxopts::Options options("./vigenere", "\nEncryp/Decrypt utf8 messages using Affine cipher\n\n"
                                         "JSON file structure:\n"
                                         "    * alphabet                The alphabet to use\n"
                                         "    * key                     The key to use to encrypt/decrypt\n"
                                         "        + a                   The a value\n"
                                         "          b                   The b value\n"
                                         "        + random              Or if you want use a random key.\n"
                                         "    + messageFile             Path to the clear message file\n"
                                         "    + encryptedMessageFile    Path to the encrypted message file\n"
                                         "      spaces                  Forces to encrypt using spaces\n"
                                         "\n* required.\n+ Either is required\n");
    // Description of the arguments                                        
    options
        .add_options()
        ("e,encrypt", "Encrypt using the given options in the .json file", cxxopts::value<std::string>(), "[JSON file]")
        ("d,decrypt", "Decrypt using the given options in the .json file", cxxopts::value<std::string>(), "[JSON file]")
        ("h,help", "Print help");

    try {

        auto result = options.parse(argc, argv);

        if (result.count("help")){
            std::cout << options.help({""}) << "\n";
        }
        else if (result.count("encrypt") || result.count("decrypt")){
            bool encrypt = result.count("encrypt") ? true : false;
            std::string jsonFile = encrypt ? result["encrypt"].as<std::string>() : result["decrypt"].as<std::string>();

            jsonParsing(jsonFile, encrypt, options);
        }
        else {
            error("Error: No arguments provided", options);
        }

    } catch (const std::exception& e){
        error(e.what(), options);
    }

    return 0;
}