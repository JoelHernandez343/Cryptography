#include <iostream>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <fstream>

#include "../../include/json.hpp"
#include "../../include/cxxopts.hpp"

#include "../../include/utf8.hpp"
#include "vigenere.hpp"

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
std::string readFromFile(std::string path, cxxopts::Options& options){
    if (!std::filesystem::exists(path)){
        error("Input file " + path + " doesn't exists.", options);
    }

    std::ifstream reader (path);
    std::istreambuf_iterator<char> it(reader), end;
    std::string file(it, end);
    reader.close();

    return file;
}

// Write the message in the given file's path
void writeToFile(std::string& message, std::string file, bool encrypt){
    
    std::string ext = encrypt ? ".vig" : ".txt";

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

    auto j = nlohmann::json::parse(readFromFile(jsonFile, options));

    if (!j.count("alphabet") || !j.count("key")){
        error("Missing json fields in " + jsonFile + ". Check it.", options);
    }

    if ((encrypt && !j.count("messageFile")) || (!encrypt && !j.count("encryptedMessageFile"))){
        std::string m = "Error: You must provide a file name in order to ";
        m.append(encrypt ? "encrypt." : "decrypt.");
        error(m, options);
    }

    std::string inputPath = encrypt ? j["messageFile"] : j["encryptedMessageFile"];
    std::string input = readFromFile(inputPath, options);

    std::string alphPath = j["alphabet"];
    std::string alphabet = readFromFile(alphPath, options);

    std::string key = j["key"];

    bool spaces = j.count("spaces") && j["spaces"] ? true : false;
    bool hasSpace = checkSpace(alphabet);

    if (!spaces && hasSpace)
        spaces = true;

    if (spaces && !hasSpace){
        alphabet.append(" ");
    }

    std::string output = encrypt ? vigenere::encrypt(input, alphabet, key, spaces) : vigenere::decrypt(input, alphabet, key, spaces);

    writeToFile(output, inputPath, encrypt);
}

int main(int argc, char * argv[]) {
   
    // Description of the application
    cxxopts::Options options("./vigenere", "\nEncryp/Decrypt utf8 messages using Vigenere cipher\n\n"
                                         "JSON file structure:\n"
                                         "    * alphabet                The file with the alphabet to use\n"
                                         "    * key                     The key to use to encrypt/decrypt\n"
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