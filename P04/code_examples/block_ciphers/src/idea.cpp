/*
 * Example of IDEA (International Data Encryption Standard by Massey) usage with CBC mode of operation
 */

#include <iostream>

#include <cryptopp/cryptlib.h> // Always
#include <cryptopp/secblock.h> // For SecByteBlock
#include <cryptopp/modes.h>    // For modes
#include <cryptopp/osrng.h>    // For AutoSeededRandomPool
#include <cryptopp/hex.h>      // For HexEncoder
#include <cryptopp/idea.h>

// Convert a byte block to its hexadecimal representation
std::string byteBlockToString(CryptoPP::SecByteBlock block) {

    std::string r;

    CryptoPP::HexEncoder hex(new CryptoPP::StringSink(r));
    hex.Put(block, block.size());
    hex.MessageEnd();

    return r;

}

// Encrypting IDEA / CBC mode
std::string encrypt(std::string input, CryptoPP::CBC_Mode<CryptoPP::IDEA>::Encryption encryptor) {
    std::string result;

    CryptoPP::StringSource ss (input, true, new CryptoPP::StreamTransformationFilter(encryptor, new CryptoPP::StringSink( result )));

    return result;
}

// Decrypting IDEA / CBC mode
std::string decrypt(std::string input, CryptoPP::CBC_Mode<CryptoPP::IDEA>::Decryption decryptor) {
    std::string result;

    CryptoPP::StringSource ss (input, true, new CryptoPP::StreamTransformationFilter(decryptor, new CryptoPP::StringSink( result )));

    return result;
}

// String to its hexadecimal representation
std::string toHex(std::string const & m){
    std::string encoded;
    
    CryptoPP::StringSource ss (m, true, new CryptoPP::HexEncoder( new CryptoPP::StringSink( encoded )));

    return encoded;
}


int main(void) {

    // Rand generator
    CryptoPP::AutoSeededRandomPool rand;

    // Generating a random key
    CryptoPP::SecByteBlock key(0x00, CryptoPP::IDEA::DEFAULT_KEYLENGTH);
    rand.GenerateBlock(key, key.size());
    std::cout << "Using key: " << byteBlockToString(key) << "\n";

    // Generating a random IV
    CryptoPP::SecByteBlock iv(0x00, CryptoPP::IDEA::BLOCKSIZE);
    rand.GenerateBlock(iv, iv.size());
    std::cout << "Using IV: " << byteBlockToString(iv) << "\n";

    // Input message
    std::string input = "Hello world!, this is a very long message to prove the IDEA encryption with CBC mode provided by the Crypto++ library!";
    std::string cipher, encoded, recovered;

    // CBC Encryptor and Decryptor
    CryptoPP::CBC_Mode<CryptoPP::IDEA>::Encryption encryptor(key, key.size(), iv);
    CryptoPP::CBC_Mode<CryptoPP::IDEA>::Decryption decryptor(key, key.size(), iv);

    // Encrypting
    cipher = encrypt(input, encryptor);
    std::cout << "Encrypted message: " << toHex(cipher) << "\n";

    // Decrypting
    recovered = decrypt(cipher, decryptor);
    std::cout << "Decrypted message: " << recovered << "\n";

    return 0;

}