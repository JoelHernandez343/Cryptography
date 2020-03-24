#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>

#include "../include/cxxopts.hpp"
#include "../include/json.hpp"

#include "permutation.hpp"

cxxopts::Options options("", "");

void exitError(std::string message){

    std::cout << "\033[40;31mERR!\033[0m " << message << "\n";
    std::cout << options.help({""}) << "\n";
    std::exit(1);

}

void printJSONdescription(){

    std::cout << "JSON configuration description: \n"
              << "\033[40;33mWARN\033[0m All the permutation's index must begin in 0\n\n"

              << "  permutation [array<int>] Define the permutation block.\n"
              
              << "  encrypt | descrypt       Define the files to work with.\n"
                 "    inputFile              Set the input file.\n"
                 "    outputFile             Set the output file.\n\n";

}

void printJSONexample(){

    std::cout << "JSON structure example:\n";

    nlohmann::json example =
    {
        {"permutation", {5, 2, 6, 3, 7, 4, 1, 0}},
        {"encrypt", {
            {"inputFile", "in.txt"},
            {"outputFile", "out.txt"}
        }},
        {"decrypt", {
            {"inputFile", "out.txt"},
            {"outputFile", "in.txt"}
        }},
    };

    std::cout << std::setw(4) << example << "\n";

}

std::string jsonValidation(nlohmann::json j, bool encrypt){

    if (!j.count("permutation"))
        return "JSON error: Missing 'permutation' field.";
    
    if (encrypt){
        
        if (!j.count("encrypt"))
            return "JSON error: Encrypting mode but 'encrypt' field is missing.";

        if (!j["encrypt"].count("inputFile"))
            return "JSON error: 'inputField' in 'encrypt' is missing.";
        
        if (!j["encrypt"].count("outputFile"))
            return "JSON error: 'outputField' in 'encrypt' is missing.";

    }
    else {

        if (!j.count("decrypt"))
            return "JSON error: Decrypting mode but 'decrypt' field is missing.";

        if (!j["decrypt"].count("inputFile"))
            return "JSON error: 'inputField' in 'decrypt' is missing.";
        
        if (!j["decrypt"].count("outputFile"))
            return "JSON error: 'outputField' in 'decrypt' is missing.";

    }

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

        std::vector<int> perm = j["permutation"];
        std::vector<int> inverse = perm::inversePermutation(perm);
        if (!inverse.size())
            exitError("Invalid permutation: No inverse property.");

        std::string inputFile  = j[encrypt ? "encrypt" : "decrypt"]["inputFile"];
        std::string outputFile = j[encrypt ? "encrypt" : "decrypt"]["outputFile"];

        std::string input  = readFromFile(inputFile);
        std::string output = perm::encrypt(input, encrypt ? perm : inverse);

        writeToFile(outputFile, output);

    } catch(std::exception const & e){

        exitError(e.what());

    } 

}

int main(int argc, char * argv[]){

    auto description = "Encrypt / Decrypt files using permutation";
    options = cxxopts::Options("permute", description);

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
            exitError("No arguments provided.");

    } catch(const cxxopts::OptionException & e){

        exitError("Error parsing arguments: " + std::string(e.what()));

    }

    return 0;

}