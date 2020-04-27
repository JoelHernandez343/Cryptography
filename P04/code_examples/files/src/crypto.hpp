/*
 * Created by Joel Hernández
 * Github: github.com/JoelHernandez343
 */

#ifndef J_HELPER_ENCRYPTION_HEADER_FUNCTIONS
#define J_HELPER_ENCRYPTION_HEADER_FUNCTIONS

#include <iostream>
#include <cryptopp/cryptlib.h>
#include <cryptopp/secblock.h>
#include <cryptopp/modes.h>
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>
#include <cryptopp/aes.h>
#include <cryptopp/des.h>
#include <cryptopp/idea.h>

namespace crypto {

    inline
    bool isValidHexChar(char c) {

        if (c == 'x' || c == 'X')
            return true;

        if (
                (c > 'f' && c <= 'z') || 
                (c > 'F' && c <= 'Z')
            )
            return false;
        
        return true;
    }

    inline
    int isValidHex(std::string hex){

        int i;

        for (i = 0; i < hex.length(); ++i)
            if (!isValidHexChar(hex[i]))
                break;
    
        return i;

    }

    inline
    int getKeySizeFromAlgorithm(std::string algorithm) {

        if (algorithm == "aes")
            return CryptoPP::AES::DEFAULT_KEYLENGTH;

        if (algorithm == "3des")
            return CryptoPP::DES_EDE3::DEFAULT_KEYLENGTH;
        
        if (algorithm == "idea")
            return CryptoPP::IDEA::DEFAULT_KEYLENGTH;
        
        return -1;

    }

    inline
    int getIvSizeFromAlgorithm(std::string algorithm) {

        if (algorithm == "aes")
            return CryptoPP::AES::BLOCKSIZE;

        if (algorithm == "3des")
            return CryptoPP::DES_EDE3::BLOCKSIZE;
        
        if (algorithm == "idea")
            return CryptoPP::IDEA::BLOCKSIZE;
        
        return -1;

    }

    inline
    CryptoPP::SecByteBlock getHexFromString(std::string const & s) {

        std::string aux;

        CryptoPP::StringSource ss( s, true, 
            new CryptoPP::HexDecoder(
                new CryptoPP::StringSink( aux )
            )
        );

        return CryptoPP::SecByteBlock((CryptoPP::byte *)aux.data(), aux.size());

    }

    std::string isValidKey(std::string key, std::string algorithm) {

        std::string error = "";

        if (crypto::isValidHex(key) != key.length()){

            error = "Provided key is malformed: \n";
            for (int i = 0; i < key.length(); ++i){

                if (crypto::isValidHexChar(key[i])) {
                    error += key[i];
                    continue;
                }

                error += "\033[40;31m";
                error += key[i];
                error += "\033[0m";

            }

            return error;

        }

        auto k = getHexFromString(key);

        if (k.size() != getKeySizeFromAlgorithm(algorithm)){

            error += "Provided key size (";
            error += std::to_string(k.size());
            error += ") is wrong, key size needed (bytes): ";
            error += std::to_string(getKeySizeFromAlgorithm(algorithm));
            error += ".";

        }

        return error;

    }

    std::string isValidIv(std::string iv, std::string algorithm) {

        std::string error = "";

        if (crypto::isValidHex(iv) != iv.length()){

            error = "Provided iv is malformed: \n";
            for (int i = 0; i < iv.length(); ++i){

                if (crypto::isValidHexChar(iv[i])) {
                    error += iv[i];
                    continue;
                }

                error += "\033[40;31m";
                error += iv[i];
                error += "\033[0m";

            }

            return error;

        }

        auto v = getHexFromString(iv);

        if (v.size() != getIvSizeFromAlgorithm(algorithm)){

            error += "Provided iv size (";
            error += std::to_string(v.size());
            error += ") is wrong, iv size needed (bytes): ";
            error += std::to_string(getIvSizeFromAlgorithm(algorithm));
            error += ".";

        }

        return error;

    }

    std::string byteBlockToString(CryptoPP::SecByteBlock block) {

        std::string r;

        CryptoPP::HexEncoder hex(new CryptoPP::StringSink(r));
        hex.Put(block, block.size());
        hex.MessageEnd();

        return r;

    }

    CryptoPP::SecByteBlock ranBlock(int length, std::string name) {

        // Rand generator
        CryptoPP::AutoSeededRandomPool rand;

        // Generating a random key
        CryptoPP::SecByteBlock key(0x00, length);
        rand.GenerateBlock(key, key.size());
        std::cout << "\033[40;33mWARN\033[0m Using random "<< name <<": " << byteBlockToString(key) << "\n";

        return key;
    }

    // Encrypt with ECB
    std::string encryptECB(std::string & in, std::string algorithm, std::string key){

        CryptoPP::SecByteBlock k;

        // Converting string to byte block or generating a random one
        if (key == "")
            k = ranBlock(getKeySizeFromAlgorithm(algorithm), "key");
        else
            k = getHexFromString(key);

        std::string result;

        std::cout << "Encrypting in mode: ECB with " << algorithm << ".\n";  

        if (algorithm == "aes") {

            CryptoPP::ECB_Mode<CryptoPP::AES>::Encryption encryptor(k, k.size());
            CryptoPP::StringSource ss(in, true, new CryptoPP::StreamTransformationFilter(encryptor, new CryptoPP::StringSink( result )));

        }

        else if (algorithm == "3des") {

            CryptoPP::ECB_Mode<CryptoPP::DES_EDE3>::Encryption encryptor(k, k.size());
            CryptoPP::StringSource ss(in, true, new CryptoPP::StreamTransformationFilter(encryptor, new CryptoPP::StringSink( result )));

        }

        else {

            CryptoPP::ECB_Mode<CryptoPP::IDEA>::Encryption encryptor(k, k.size());
            CryptoPP::StringSource ss(in, true, new CryptoPP::StreamTransformationFilter(encryptor, new CryptoPP::StringSink( result )));

        }

        return result;

    }

     // Decrypt with ECB
    std::string decryptECB(std::string & in, std::string algorithm, std::string key){

        // Converting string to byte block
        CryptoPP::SecByteBlock k = getHexFromString(key);

        std::string result;

        std::cout << "Decrypting in mode: ECB with " << algorithm << ".\n";  

        if (algorithm == "aes") {

            CryptoPP::ECB_Mode<CryptoPP::AES>::Decryption d(k, k.size());
            CryptoPP::StringSource ss(in, true, new CryptoPP::StreamTransformationFilter(d, new CryptoPP::StringSink( result )));

        }

        else if (algorithm == "3des") {

            CryptoPP::ECB_Mode<CryptoPP::DES_EDE3>::Decryption d(k, k.size());
            CryptoPP::StringSource ss(in, true, new CryptoPP::StreamTransformationFilter(d, new CryptoPP::StringSink( result )));

        }

        else {

            CryptoPP::ECB_Mode<CryptoPP::IDEA>::Decryption d(k, k.size());
            CryptoPP::StringSource ss(in, true, new CryptoPP::StreamTransformationFilter(d, new CryptoPP::StringSink( result )));

        }

        return result;

    }

}

#endif