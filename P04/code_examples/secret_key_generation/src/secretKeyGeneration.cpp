/*
 * Key generation example for AES and an iv intialization.
 */

#include <iostream>

#include <cryptopp/cryptlib.h>
#include <cryptopp/secblock.h>
#include <cryptopp/osrng.h>
#include <cryptopp/aes.h>
#include <cryptopp/hex.h>

int main(void) {

    // We declare a byte block of AES::DEFAULT_KEYLENGTH (16 bytes)
    CryptoPP::SecByteBlock key(CryptoPP::AES::DEFAULT_KEYLENGTH);
    // Where will be storage
    std::string k;

    // Fill with OS entropy
    CryptoPP::OS_GenerateRandomBlock(true, key, key.size());

    // Hex encoder to k
    CryptoPP::HexEncoder hex(new CryptoPP::StringSink(k));
    hex.Put(key, key.size());
    hex.MessageEnd();

    // Now with the iv
    CryptoPP::SecByteBlock iv(CryptoPP::AES::BLOCKSIZE);
    CryptoPP::OS_GenerateRandomBlock(false, iv, iv.size());
    std::string v;
    hex.Detach(new CryptoPP::StringSink(v));
    hex.Put(iv, iv.size());
    hex.MessageEnd();

    std::cout << "Key: " << k << "\n";
    std::cout << "IV: " << v << "\n";

    return 0;

}
