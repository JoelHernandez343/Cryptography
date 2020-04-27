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


    template<class T>
    struct modes {
        typename CryptoPP::ECB_Mode<T>::Encryption ecbE;
        typename CryptoPP::CBC_Mode<T>::Encryption cbcE;
        typename CryptoPP::CTR_Mode<T>::Encryption ctrE;
        typename CryptoPP::OFB_Mode<T>::Encryption ofbE;
        typename CryptoPP::CFB_Mode<T>::Encryption cfbE;

        typename CryptoPP::ECB_Mode<T>::Decryption ecbD;
        typename CryptoPP::CBC_Mode<T>::Decryption cbcD;
        typename CryptoPP::CTR_Mode<T>::Decryption ctrD;
        typename CryptoPP::OFB_Mode<T>::Decryption ofbD;
        typename CryptoPP::CFB_Mode<T>::Decryption cfbD;
    };

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
    int getKeySize(std::string algorithm) {

        if (algorithm == "aes")
            return CryptoPP::AES::DEFAULT_KEYLENGTH;

        if (algorithm == "3des")
            return CryptoPP::DES_EDE3::DEFAULT_KEYLENGTH;
        
        if (algorithm == "idea")
            return CryptoPP::IDEA::DEFAULT_KEYLENGTH;
        
        return -1;

    }

    inline
    int getIvSize(std::string algorithm) {

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

        if (k.size() != getKeySize(algorithm)){

            error += "Provided key size (";
            error += std::to_string(k.size());
            error += ") is wrong, key size needed (bytes): ";
            error += std::to_string(getKeySize(algorithm));
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

        if (v.size() != getIvSize(algorithm)){

            error += "Provided iv size (";
            error += std::to_string(v.size());
            error += ") is wrong, iv size needed (bytes): ";
            error += std::to_string(getIvSize(algorithm));
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

    // Really ugly implementation, I can't use functions because the hierarchy class tree of Crypto++

    inline
    std::string encrypt(std::string & input, std::string algorithm, std::string mode, std::string key, std::string iv){

        CryptoPP::SecByteBlock k = key == "" ? ranBlock(getKeySize(algorithm), "key") : getHexFromString(key);
        CryptoPP::SecByteBlock v = (mode != "ecb" && iv == "") ? ranBlock(getIvSize(algorithm), "iv") : getHexFromString(iv);
        
        std::string result;

        std::cout << "Encrypting in mode: " << mode << " with: " << algorithm << ".\n";
        
        if (algorithm == "aes") {
            auto l = modes<CryptoPP::AES>();
            
            if (mode == "ecb"){

                auto e = l.ecbE;
                mode == "ecb" ? e.SetKey(k, k.size()) : e.SetKeyWithIV(k, k.size(), v);
                CryptoPP::StringSource ss(input, true, new CryptoPP::StreamTransformationFilter(e, new CryptoPP::StringSink( result )));

            }
            
            if (mode == "cbc"){

                auto e = l.cbcE;
                e.SetKeyWithIV(k, k.size(), v);
                CryptoPP::StringSource ss(input, true, new CryptoPP::StreamTransformationFilter(e, new CryptoPP::StringSink( result )));
                
            }

            if (mode == "ctr"){

                auto e = l.ctrE;
                e.SetKeyWithIV(k, k.size(), v);
                CryptoPP::StringSource ss(input, true, new CryptoPP::StreamTransformationFilter(e, new CryptoPP::StringSink( result )));
                
            }

            if (mode == "ofb"){

                auto e = l.ofbE;
                e.SetKeyWithIV(k, k.size(), v);
                CryptoPP::StringSource ss(input, true, new CryptoPP::StreamTransformationFilter(e, new CryptoPP::StringSink( result )));
                
            }

            if (mode == "cfb"){

                auto e = l.cfbE;
                e.SetKeyWithIV(k, k.size(), v);
                CryptoPP::StringSource ss(input, true, new CryptoPP::StreamTransformationFilter(e, new CryptoPP::StringSink( result )));
                
            }
        }

        if (algorithm == "3des") {
            auto l = modes<CryptoPP::DES_EDE3>();
            
            if (mode == "ecb"){

                auto e = l.ecbE;
                mode == "ecb" ? e.SetKey(k, k.size()) : e.SetKeyWithIV(k, k.size(), v);
                CryptoPP::StringSource ss(input, true, new CryptoPP::StreamTransformationFilter(e, new CryptoPP::StringSink( result )));

            }
            
            if (mode == "cbc"){

                auto e = l.cbcE;
                e.SetKeyWithIV(k, k.size(), v);
                CryptoPP::StringSource ss(input, true, new CryptoPP::StreamTransformationFilter(e, new CryptoPP::StringSink( result )));
                
            }

            if (mode == "ctr"){

                auto e = l.ctrE;
                e.SetKeyWithIV(k, k.size(), v);
                CryptoPP::StringSource ss(input, true, new CryptoPP::StreamTransformationFilter(e, new CryptoPP::StringSink( result )));
                
            }

            if (mode == "ofb"){

                auto e = l.ofbE;
                e.SetKeyWithIV(k, k.size(), v);
                CryptoPP::StringSource ss(input, true, new CryptoPP::StreamTransformationFilter(e, new CryptoPP::StringSink( result )));
                
            }

            if (mode == "cfb"){

                auto e = l.cfbE;
                e.SetKeyWithIV(k, k.size(), v);
                CryptoPP::StringSource ss(input, true, new CryptoPP::StreamTransformationFilter(e, new CryptoPP::StringSink( result )));
                
            }
        }

        if (algorithm == "idea") {
            auto l = modes<CryptoPP::IDEA>();
            
            if (mode == "ecb"){

                auto e = l.ecbE;
                mode == "ecb" ? e.SetKey(k, k.size()) : e.SetKeyWithIV(k, k.size(), v);
                CryptoPP::StringSource ss(input, true, new CryptoPP::StreamTransformationFilter(e, new CryptoPP::StringSink( result )));

            }
            
            if (mode == "cbc"){

                auto e = l.cbcE;
                e.SetKeyWithIV(k, k.size(), v);
                CryptoPP::StringSource ss(input, true, new CryptoPP::StreamTransformationFilter(e, new CryptoPP::StringSink( result )));
                
            }

            if (mode == "ctr"){

                auto e = l.ctrE;
                e.SetKeyWithIV(k, k.size(), v);
                CryptoPP::StringSource ss(input, true, new CryptoPP::StreamTransformationFilter(e, new CryptoPP::StringSink( result )));
                
            }

            if (mode == "ofb"){

                auto e = l.ofbE;
                e.SetKeyWithIV(k, k.size(), v);
                CryptoPP::StringSource ss(input, true, new CryptoPP::StreamTransformationFilter(e, new CryptoPP::StringSink( result )));
                
            }

            if (mode == "cfb"){

                auto e = l.cfbE;
                e.SetKeyWithIV(k, k.size(), v);
                CryptoPP::StringSource ss(input, true, new CryptoPP::StreamTransformationFilter(e, new CryptoPP::StringSink( result )));
                
            }
        }

        return result;

    }

    inline
    std::string decrypt(std::string & input, std::string algorithm, std::string mode, std::string key, std::string iv){

        CryptoPP::SecByteBlock k = getHexFromString(key);
        CryptoPP::SecByteBlock v = getHexFromString(iv);
        
        std::string result;

        std::cout << "Decrypting in mode: " << mode << " with: " << algorithm << ".\n";
        
        if (algorithm == "aes") {
            auto l = modes<CryptoPP::AES>();
            
            if (mode == "ecb"){

                auto e = l.ecbD;
                e.SetKey(k, k.size());
                CryptoPP::StringSource ss(input, true, new CryptoPP::StreamTransformationFilter(e, new CryptoPP::StringSink( result )));

            }
            
            if (mode == "cbc"){

                auto e = l.cbcD;
                e.SetKeyWithIV(k, k.size(), v);
                CryptoPP::StringSource ss(input, true, new CryptoPP::StreamTransformationFilter(e, new CryptoPP::StringSink( result )));
                
            }

            if (mode == "ctr"){

                auto e = l.ctrD;
                e.SetKeyWithIV(k, k.size(), v);
                CryptoPP::StringSource ss(input, true, new CryptoPP::StreamTransformationFilter(e, new CryptoPP::StringSink( result )));
                
            }

            if (mode == "ofb"){

                auto e = l.ofbD;
                e.SetKeyWithIV(k, k.size(), v);
                CryptoPP::StringSource ss(input, true, new CryptoPP::StreamTransformationFilter(e, new CryptoPP::StringSink( result )));
                
            }

            if (mode == "cfb"){

                auto e = l.cfbD;
                e.SetKeyWithIV(k, k.size(), v);
                CryptoPP::StringSource ss(input, true, new CryptoPP::StreamTransformationFilter(e, new CryptoPP::StringSink( result )));
                
            }
        }

        if (algorithm == "3des") {
            auto l = modes<CryptoPP::DES_EDE3>();
            
            if (mode == "ecb"){

                auto e = l.ecbD;
                e.SetKey(k, k.size());
                CryptoPP::StringSource ss(input, true, new CryptoPP::StreamTransformationFilter(e, new CryptoPP::StringSink( result )));

            }
            
            if (mode == "cbc"){

                auto e = l.cbcD;
                e.SetKeyWithIV(k, k.size(), v);
                CryptoPP::StringSource ss(input, true, new CryptoPP::StreamTransformationFilter(e, new CryptoPP::StringSink( result )));
                
            }

            if (mode == "ctr"){

                auto e = l.ctrD;
                e.SetKeyWithIV(k, k.size(), v);
                CryptoPP::StringSource ss(input, true, new CryptoPP::StreamTransformationFilter(e, new CryptoPP::StringSink( result )));
                
            }

            if (mode == "ofb"){

                auto e = l.ofbD;
                e.SetKeyWithIV(k, k.size(), v);
                CryptoPP::StringSource ss(input, true, new CryptoPP::StreamTransformationFilter(e, new CryptoPP::StringSink( result )));
                
            }

            if (mode == "cfb"){

                auto e = l.cfbD;
                e.SetKeyWithIV(k, k.size(), v);
                CryptoPP::StringSource ss(input, true, new CryptoPP::StreamTransformationFilter(e, new CryptoPP::StringSink( result )));
                
            }
        }

        if (algorithm == "idea") {
            auto l = modes<CryptoPP::IDEA>();
            
            if (mode == "ecb"){

                auto e = l.ecbD;
                e.SetKey(k, k.size());
                CryptoPP::StringSource ss(input, true, new CryptoPP::StreamTransformationFilter(e, new CryptoPP::StringSink( result )));

            }
            
            if (mode == "cbc"){

                auto e = l.cbcD;
                e.SetKeyWithIV(k, k.size(), v);
                CryptoPP::StringSource ss(input, true, new CryptoPP::StreamTransformationFilter(e, new CryptoPP::StringSink( result )));
                
            }

            if (mode == "ctr"){

                auto e = l.ctrD;
                e.SetKeyWithIV(k, k.size(), v);
                CryptoPP::StringSource ss(input, true, new CryptoPP::StreamTransformationFilter(e, new CryptoPP::StringSink( result )));
                
            }

            if (mode == "ofb"){

                auto e = l.ofbD;
                e.SetKeyWithIV(k, k.size(), v);
                CryptoPP::StringSource ss(input, true, new CryptoPP::StreamTransformationFilter(e, new CryptoPP::StringSink( result )));
                
            }

            if (mode == "cfb"){

                auto e = l.cfbD;
                e.SetKeyWithIV(k, k.size(), v);
                CryptoPP::StringSource ss(input, true, new CryptoPP::StreamTransformationFilter(e, new CryptoPP::StringSink( result )));
                
            }
        }

        return result;

    }

}

#endif