#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>

#include <iomanip>

#include "../include/cxxopts.hpp"
#include "../include/json.hpp"

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

    std::cout << "JSON configuration description: \n";

    std::cout << "\033[40;33mWARN\033[0m All the permutation's index must begin in 0\n\n";

    std::cout << "  modeOfOperation [simple, ...] \n"
                 "                          Set the mode of operation.\n\n";

    std::cout << "  encrypt | descrypt      Define the files to work with.\n"
                 "    inputFile             Set the input file.\n"
                 "    outputFile            Set the output file.\n\n";

    std::cout << "  keyConfig               Define the key config.\n"
                 "    key                   Binarty form of the key.\n"
                 "    p10 [array: 10]       The 10 permutation.\n"
                 "    p8 [array: 8]         The compression permutation\n\n";

    std::cout << "  cryptConfig             The main process config.\n"
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
        {"modeOfOperation", "simple"},
        {"encrypt", {
            {"inputFile", "message.txt"},
            {"outputFile", "message.sdes"}
        }},
        {"decrypt", {
            {"inputFile", "message.sdes"},
            {"outputFile", "message.txt"}
        }},
        {"keyConfig", {
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

void jsonParsing(std::string jsonFile, bool encrypt){

    if (!std::filesystem::exists(jsonFile))
        exitError("JSON file " + jsonFile + "doesn't exists.");

    std::ifstream reader(jsonFile);
    std::istreambuf_iterator<char> it(reader), end;
    std::string tmp (it, end);
    reader.close();

    nlohmann::json json = nlohmann::json::parse(tmp);

    if (!json.count("modeOfOperation"))
        exitError("JSON error: missing 'modeOfOperation' field.");
    std::string operation = json["modeOfOperation"];

    if (encrypt && (!json.count("encrypt") || !json["encrypt"].count("inputFile")))
        std::cout << "continuara...\n";

}

int main(int argc, char * argv[]){

    auto description = "Encrypt / Decrypt files using Simplified DES\n";
    options = cxxopts::Options("sdes", description);

    options
        .add_options()
        ("e,encrypt", "Encrypt using the given options in .json file.", cxxopts::value<std::string>(), "[JSON FILE]" )
        ("d,decrypt", "Decrypt using the given options in .json file.", cxxopts::value<std::string>(), "[JSON FILE]")
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

            const auto & v = result["json"].as<std::string>();

            if (v == "all"){
                
                printJSONdescription();
                printJSONexample();
                
            }
            else if (v == "desc")
                printJSONdescription();
            
            else if (v == "example")
                printJSONexample();
            
            else 
                exitError("Invalid --json option:" + v[0]);

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