#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>

#include <cstdlib>
#include <ctime>

#include "../include/cxxopts.hpp"
#include "../include/json.hpp"

#include "operation_mode.hpp"
#include "bitperm.hpp"
#include "sdes.hpp"

#include "helpers.hpp"

cxxopts::Options options ("", "");

void exitError(std::string message){

    std::cout << "\033[40;31mERR!\033[0m " << message << "\n";
    std::cout << options.help({""}) << "\n";
    std::exit(1);

}

void printJSONdescription(){

    std::cout << "JSON configuration description: \n"
              << "\033[40;33mWARN\033[0m All the permutation's index must begin in 0\n\n"

              << "  iv                      The initialization vector. Used for decrypt.\n"

              << "  encrypt | descrypt      Define the files to work with.\n"
                 "    inputFile             Set the input file.\n"
                 "    outputFile            Set the output file.\n\n"

              << "  keyConfig               Define the key config.\n"
                 "    random [bool]         Define random mode key for encryption.\n"
                 "    key                   Binary form of the key. Necesary for decryption.\n"
                 "    p10 [array: 10]       The 10 permutation.\n"
                 "    p8 [array: 8]         The compression permutation\n\n"

              << "  cryptConfig             The main process config.\n"
                 "    initialPermutation [array: 8]\n"
                 "                          Initial permutation\n"
                 "    expansion [array: 8 ] Expansion permutation\n"
                 "    p4 [array: 4]         The 4 permutation\n"
                 "    s0: [array<array: 4>: 4]\n"
                 "                          The s0 box\n"
                 "    s1: [array<array: 4>: 4]\n"
                 "                          The s1 box\n\n";

}

void printJSONexample(){

    std::cout << "JSON structure example:\n";

    nlohmann::json example = 
    {
        {"iv", "10100001"},
        {"encrypt", {
            {"inputFile", "message.txt"},
            {"outputFile", "message.sdes"}
        }},
        {"decrypt", {
            {"inputFile", "message.sdes"},
            {"outputFile", "message.txt"}
        }},
        {"keyConfig", {
            {"random", true},
            {"key", "1010000010"},
            {"p10", {2, 4, 1, 6, 3, 9, 0, 8, 7, 5}},
            {"p8", {5, 2, 6, 3, 7, 4, 9, 8}}
        }},
        {"cryptConfig", {
            {"initialPermutation", {1, 5, 2, 0, 3, 7, 4, 6}},
            {"expansion", {3, 0, 1, 2, 1, 2, 3, 0}},
            {"p4", {1, 3, 2, 0}},
            {"s0", {
                {1, 0, 3, 2},
                {3, 2, 1, 0},
                {0, 2, 1, 3},
                {3, 1, 3, 2}
            }},
            {"s1", {
                {0, 1, 2, 3},
                {2, 0, 1, 3},
                {3, 0, 1, 0},
                {2, 1, 0, 3}
            }}
        }}
    };

    std::cout << std::setw(4) << example << "\n";

}

std::string jsonValidation(nlohmann::json j, bool encrypt){
    
    if (!j.count("keyConfig"))
        return "JSON error: Missing 'keyConfig' field.";

    if (!j["keyConfig"].count("p10"))
        return "JSON error: Missing 'p10' in 'keyConfig'";
    
    if (!j["keyConfig"].count("p8"))
        return "JSON error: Missing 'p8' in 'keyConfig'";
    
    if (encrypt){
        
        if (!j.count("encrypt"))
            return "JSON error: Encrypting mode but 'encrypt' field is missing.";

        if (!j["encrypt"].count("inputFile"))
            return "JSON error: 'inputField' in 'encrypt' is missing.";
        
        if (!j["encrypt"].count("outputFile"))
            return "JSON error: 'outputField' in 'encrypt' is missing.";

        if ((!j["keyConfig"].count("random") || !j["keyConfig"]["random"]) && !j["keyConfig"].count("key"))
            return "JSON error: Random mode but 'key' in 'keyConfig' is missing.";

    }
    else {
        if (!j.count("iv"))
            return "JSON error: Decrypting mode but 'iv' is not provided.";

        if (!j.count("decrypt"))
            return "JSON error: Decrypting mode but 'decrypt' field is missing.";

        if (!j["decrypt"].count("inputFile"))
            return "JSON error: 'inputField' in 'decrypt' is missing.";
        
        if (!j["decrypt"].count("outputFile"))
            return "JSON error: 'outputField' in 'decrypt' is missing.";

        if (!j["keyConfig"].count("key"))
            return "JSON error: Decrypting mode but 'key' in 'keyConfig' is missing.";

    }

    if (!j.count("cryptConfig"))
        return "JSON error: Missing 'keyConfig' field.";

    if (!j["cryptConfig"].count("initialPermutation"))
        return "JSON error: Missing 'initialPermutation' in 'cryptConfig'";
    
    if (!j["cryptConfig"].count("expansion"))
        return "JSON error: Missing 'expansion' in 'cryptConfig'";

    if (!j["cryptConfig"].count("p4"))
        return "JSON error: Missing 'p4' in 'cryptConfig'";
    
    if (!j["cryptConfig"].count("s0"))
        return "JSON error: Missing 's0' in 'cryptConfig'";
    
    if (!j["cryptConfig"].count("s1"))
        return "JSON error: Missing 's1' in 'cryptConfig'";

    return "";

}

auto readFromFile(std::string & file){

    if (!std::filesystem::exists(file))
        exitError("File " + file + " doesn't exists.");
    
    if (std::filesystem::is_directory(file))
        exitError("File name " + file + " is a directory.");

    std::ifstream reader(file);
    std::istreambuf_iterator<char> it(reader), end;

    return std::string(it, end);

}

void writeToFile(std::string & file, std::string & content){

    std::ofstream writer(file, std::ios::trunc);
    writer << content;
    writer.close();

    std::cout << "\033[40;32mDONE\033[0m Output file: " << file << "\n";

}

void jsonParsing(std::string jsonFile, bool encrypt){

    if (!std::filesystem::exists(jsonFile))
        exitError("JSON file " + jsonFile + " doesn't exists.");
    
    if (std::filesystem::is_directory(jsonFile))
        exitError("JSON file name " + jsonFile + " is a directory.");

    try {

        nlohmann::json j = nlohmann::json::parse(readFromFile(jsonFile));

        if (auto m = jsonValidation(j, encrypt); m != "")
            exitError(m);
        
        unsigned char iv;
        if (encrypt){

            iv = help::randomBits(8);
            std::cout << "\033[40;33mWARN\033[0m Using the random IV:  ";
            help::printBits(iv, 8);

        }
        else
            iv = help::stringToBinary(j["iv"]);

        bool random;
        if (j["keyConfig"].count("random"))
            random = j["keyConfig"]["random"];
        else
            random = false;

        unsigned short key;
        if (random){
            
            key = help::randomBits(10);
            std::cout << "\033[40;33mWARN\033[0m Using the random key: ";
            help::printBits(key, 10);

        }
        else
            key = help::stringToBinary(j["keyConfig"]["key"]);
        
        std::vector<char> p10 = j["keyConfig"]["p10"];
        std::vector<char> p8  = j["keyConfig"]["p8"];

        std::vector<char> ip  = j["cryptConfig"]["initialPermutation"];
        std::vector<char> exp = j["cryptConfig"]["expansion"];
        std::vector<char> p4 =  j["cryptConfig"]["p4"];

        std::vector<std::vector<char>> s0 = j["cryptConfig"]["s0"];
        std::vector<std::vector<char>> s1 = j["cryptConfig"]["s1"];

        std::string inputFile  = j[encrypt ? "encrypt" : "decrypt"]["inputFile"];
        std::string outputFile = j[encrypt ? "encrypt" : "decrypt"]["outputFile"];

        std::string input  = readFromFile(inputFile);
        std::string output = encrypt ? 
                             opmode::encryptCBC(input, iv, ip, exp, s0, s1, p4, key, p10, p8)
                             :
                             opmode::decryptCBC(input, iv, ip, exp, s0, s1, p4, key, p10, p8);
        
        writeToFile(outputFile, output);
        

    } catch (std::exception const & e){

        exitError(e.what());

    }
}

int main(int argc, char * argv[]){
    srand(time(NULL));

    auto description = "Encrypt / Decrypt files using Simplified DES and CBC mode (for now)\n";
    options = cxxopts::Options("sdes", description);

    options
        .add_options()
        ("e,encrypt", "Encrypt using given options in .json file.", cxxopts::value<std::string>(), "[JSON FILE]")
        ("d,decrypt", "Decrypt using given options in .json file.", cxxopts::value<std::string>(), "[JSON FILE]")
        ("json", "View a .json example and/or its description.", cxxopts::value<std::string>(), "[all | example | desc]")
        ("h,help", "Print this help.");

    try {

        auto result = options.parse(argc, argv);

        if (result.count("help"))
            std::cout << options.help({""}) << "\n";
        
        else if (result.count("encrypt")){

            auto jsonFile = result["encrypt"].as<std::string>();
            jsonParsing(jsonFile, true);
            
        }

        else if (result.count("decrypt")){

            auto jsonFile = result["decrypt"].as<std::string>();
            jsonParsing(jsonFile, false);
        
        }

        else if (result.count("json")){

            const auto & val = result["json"].as<std::string>();

            if (val == "all"){
                
                printJSONdescription();
                printJSONexample();
                
            }

            else if (val == "desc")
                printJSONdescription();
            
            else if (val == "example")
                printJSONexample();
            
            else 
                exitError("Invalid --json option:" + val);

        }

        else 
            exitError("No arguments provided");

    } catch (const cxxopts::OptionException & e){

        exitError("Error parsing arguments: " + std::string(e.what()));

    }



    // unsigned char  m = 0b10111101;
    // unsigned short k = 0b1010000010;

    // std::vector<char> ip  = {1, 5, 2, 0, 3, 7, 4, 6};
    // std::vector<char> exp = {3, 0, 1, 2, 1, 2, 3, 0};

    // char s0[4][4] = {
    //     {1, 0, 3, 2},
    //     {3, 2, 1, 0},
    //     {0, 2, 1, 3},
    //     {3, 1, 3, 2}
    // };

    // char s1[4][4] = {
    //     {0, 1, 2, 3},
    //     {2, 0, 1, 3},
    //     {3, 0, 1, 0},
    //     {2, 1, 0, 3}
    // };

    // std::vector<char> p4  = {1, 3, 2, 0};
    // std::vector<char> p10 = {2, 4, 1, 6, 3, 9, 0, 8, 7, 5};
    // std::vector<char> p8  = {5, 2, 6, 3, 7, 4, 9, 8};

    // auto c =  sdes::encrypt(m, ip, exp, s0, s1, p4, k, p10, p8);

    // help::printBits(m, 8);
    // help::printBits(c, 8);

    // auto mm = sdes::decrypt(c, ip, exp, s0, s1, p4, k, p10, p8);

    // help::printBits(mm, 8);

    return 0;

}