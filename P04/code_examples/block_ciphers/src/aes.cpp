/*
 * Example of AES usage with CBC mode of operation
 */

#include <iostream>

#include <cryptopp/cryptlib.h> // Always
#include <cryptopp/secblock.h> // For SecByteBlock
#include <cryptopp/modes.h>    // For modes
#include <cryptopp/osrng.h>    // For AutoSeededRandomPool
#include <cryptopp/hex.h>      // For HexEncoder
#include <cryptopp/aes.h>

std::string byteBlockToString(CryptoPP::SecByteBlock block) {

    std::string r;

    CryptoPP::HexEncoder hex(new CryptoPP::StringSink(r));
    hex.Put(block, block.size());
    hex.MessageEnd();

    return r;

}

int main(void) {

    // Rand generator
    CryptoPP::AutoSeededRandomPool rand;

    // Generating a random key
    CryptoPP::SecByteBlock key(0x00, CryptoPP::AES::DEFAULT_KEYLENGTH);
    rand.GenerateBlock(key, key.size());
    //std::cout << "Using key: " << byteBlockToString(key) << "\n";

    // Generating a random IV
    CryptoPP::SecByteBlock iv(0x00, CryptoPP::AES::BLOCKSIZE);
    rand.GenerateBlock(iv, iv.size());
    //std::cout << "Using IV: " << byteBlockToString(iv) << "\n";

    // Input message
    std::string input = "Hello world!, this is a very long message to prove the AES encryption with CBC mode provided by the Crypto++ library!";
    std::string cipher, encoded, recovered;


    CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption encryptor(key, key.size(), iv);
    CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption decryptor(key, key.size(), iv);

    try {

        CryptoPP::StringSource ss (input, true, new CryptoPP::StreamTransformationFilter(encryptor, new CryptoPP::StringSink( cipher )));

    } catch (CryptoPP::Exception & e){

        std::cerr << e.what() << "\n";
        std::exit(1);

    }

    CryptoPP::StringSource ss (cipher, true, new CryptoPP::HexEncoder( new CryptoPP::StringSink( encoded )));
    std::cout << "Encrypted message: " << encoded << "\n";

    try {

        CryptoPP::StringSource ss (cipher, true, new CryptoPP::StreamTransformationFilter(decryptor, new CryptoPP::StringSink( recovered )));

    } catch (CryptoPP::Exception & e){

        std::cerr << e.what() << "\n";
        std::exit(1);

    }

    std::cout << "Decrypted message: " << recovered << "\n";

    return 0;

}