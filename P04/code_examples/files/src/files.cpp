#include <iostream>
#include <fstream>
#include <sstream> 
#include <iomanip>
#include <string>

#include "crypto.hpp"

#include "../include/cxxopts.hpp"
#include "../include/json.hpp"

cxxopts::Options options("files", "Testing Crypto++ for file encryption.");

void exitError(std::string message) {

    std::cout << "\033[40;31mERR!\033[0m " << message << "\n\n";
    std::cout << options.help({""}) << "\n";
    std::exit(1);

}

auto readFromFile(std::string fileName) {

    std::ifstream reader(fileName);
    std::istreambuf_iterator<char> it(reader), end;

    return std::string(it, end);

}

void writeToFile(std::string fileName, std::string content) {

    std::ofstream writer(fileName, std::ios::trunc);
    writer << content;
    writer.close();

    std::cout << "\033[40;32mDONE\033[0m Output file: " << fileName << "\n";

}

void createJsonExample() {

    std::cout << "Creating JSON file example (config.json)...\n";

    nlohmann::json example = 
    {
        {"mode", "cbc"},
        {"algorithm", "aes"},
        {"key", "8C0581E4BCAD5D1FF4FA669F63B8FC81"},
        {"iv", "FA57ECAE32CC502BD91F6CAF2476C151"},
        {"encrypt", {
            {"inputFile", "in.txt"},
            {"outputFile", "out.txt"}
        }},
        {"decrypt", {
            {"inputFile", "out.txt"},
            {"outputFile", "in.txt"}
        }},
    };

    std::stringstream ss;
    ss << std::setw(4) << example;

    writeToFile("config.json", ss.str());

}

void printJsonDescription() {

    std::cout << "JSON configuration description: \n"

              << "  algorithm           aes | 3des | idea\n"
              << "                      Algorithm to use.\n\n"

              << "  mode                ecb | cbc | ctr | ofb | cfb\n"
              << "                      Mode of operation.\n\n"

              << "  key                 Hexadecimal representation of key\n"
              << "                      Length must match with algorithm key length.\n"
              << "                      \033[40;33mWARN\033[0m If is not indicated, will be \n"
              << "                        generated randomly on encrypting mode.\n"
              << "                      \033[40;33mWARN\033[0m Must be present on decrypting mode.\n\n"

              << "  iv                  Hexadecimal representation of iv (not occupied by ecb)\n"
              << "                      Length must match with algorithm key length.\n\n"
              << "                      \033[40;33mWARN\033[0m If is not indicated, will be \n"
              << "                        generated randomly on encrypting mode.\n"
              << "                      \033[40;33mWARN\033[0m Must be present on decrypting mode.\n\n"
              
              << "  encrypt | descrypt  Define the files to work with.\n"
                 "    inputFile         Set the input file.\n"
                 "    outputFile        Set the output file.\n\n";

}

std::string jsonValidation(nlohmann::json j, bool encrypt) {

    if (!j.count("algorithm"))
        return "JSON error: Missing 'algorithm' field.";
    
    std::string algorithm = j["algorithm"];
    if (algorithm != "aes" && algorithm != "3des" && algorithm != "idea")
        return "JSON error: Algorithm: '" + algorithm + "' not supported. Only aes | 3des | idea";
    
    if (!j.count("mode"))
        return "JSON error: Missing 'mode' field.";

    std::string mode = j["mode"];
    if (mode != "ecb" && 
        mode != "cbc" && 
        mode != "ctr" &&
        mode != "ofb" &&
        mode != "cfb"
    )
        return "JSON error: Mode: '" + mode + "' not supported. Only ecb | cbc | ctr | ofb | cfb";

    std::string cryptMode = encrypt ? "encrypt" : "decrypt";
    std::string aux = (encrypt ? "Encrypting" : "Decrypting");
        
    if (!j.count(cryptMode))
        return "JSON error: " + aux + " mode but '" + cryptMode + "' field is missing.";

    if (!j[cryptMode].count("inputFile"))
        return "JSON error: 'inputField' in '" + cryptMode + "' is missing.";
    
    if (!j[cryptMode].count("outputFile"))
        return "JSON error: 'outputField' in '" + cryptMode + "' is missing.";

    if (!encrypt) {

        if (!j.count("key"))
            return "JSON error: Decrypting mode but 'key' is missing.";
        
        if (mode != "ecb" && !j.count("iv"))
            return "JSON error: Decrypting mode and mode of operation selected is '" + mode + "' but 'iv' is missing.";

    }

    return "";

}

void jsonParsing(std::string jsonFileName, bool encrypt) {

    try {

        nlohmann::json j = nlohmann::json::parse(readFromFile(jsonFileName));
        
        std::string m = jsonValidation(j, encrypt);
        if (m != "")
            exitError(m);

        if (!encrypt || j.count("key")){

            m = crypto::isValidKey(j["key"], j["algorithm"]);
            if (m != "")
                exitError(m);

        }

        if (j["mode"] != "ecb" && (!encrypt || j.count("iv"))){

            m = crypto::isValidIv(j["iv"], j["algorithm"]);
            if (m != "")
                exitError(m);

        }


        std::string algorithm = j["algorithm"];
        std::string mode = j["mode"];
        std::string key = j.count("key") ? j["key"] : "";
        std::string iv = (mode != "ecb" && j.count("iv")) ? j["iv"] : "";

        std::string inputFile  = j[encrypt ? "encrypt" : "decrypt"]["inputFile"];
        std::string outputFile = j[encrypt ? "encrypt" : "decrypt"]["outputFile"];

        std::string input  = readFromFile(inputFile);
        std::string output = encrypt ? crypto::encrypt(input, algorithm, mode, key, iv) : crypto::decrypt(input, algorithm, mode, key, iv);

        writeToFile(outputFile, output);


    } catch (std::exception const & e) { exitError(e.what()); }

}

void validOptions(cxxopts::ParseResult result) {

    if (result.count("help"))
        std::cout << options.help({""}) << "\n";
    
    else if (result.count("json")) {

        std::string jsonOption = result["json"].as<std::string>();

        if (jsonOption == "example")
            createJsonExample();
        else if (jsonOption == "desc")
            printJsonDescription();
        else
            exitError("Invalid --json argument: " + jsonOption);

    }

    else if (result.count("encrypt"))
        jsonParsing(result["encrypt"].as<std::string>(), true);

    else if (result.count("decrypt"))
        jsonParsing(result["decrypt"].as<std::string>(), false);

    else
        exitError("No valid arguments provided.");

}

int main (int argc, char * argv[]) {

    options
        .add_options()
        ("e,encrypt", "Encrypt with options given in file.", cxxopts::value<std::string>(), "[JSON FILE]")
        ("d,decrypt", "Decrypt with options given in file.", cxxopts::value<std::string>(), "[JSON FILE]")
        ("j,json", "Create a config.json", cxxopts::value<std::string>(), "[example | desc]")
        ("h,help", "Print this help");
    
    try {

        validOptions( options.parse(argc, argv) );

    } catch (cxxopts::OptionException const & e) { exitError(e.what()); }

    return 0;

}