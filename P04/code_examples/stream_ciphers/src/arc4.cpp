/*
 * ARC4 Stream Cipher example
 * All availables:
 * XChaCha20Poly1305
 * ChaCha20Poly1305
 * XChaCha20
 * XChaCha20
 * ChaChaTLS
 * ChaCha20
 * Salsa20
 * Sosemanuk
 * Panama
 * HC-256
 * HC-128
 * Rabbit
 * ARC4
 * SEAL
 * WAKE
 * WAKE-OFB
 * BlumBlumShub
 * 
 */

#include <iostream>

#include <cryptopp/cryptlib.h>
#include <cryptopp/arc4.h>

int main(void) {

    std::string message = "Hello world";

    std::string key = "123"; // Could be random generated with SecBlock and OS_GenerateRandomBlock

    CryptoPP::ARC4 arc4((CryptoPP::byte*)key.data(), key.size());
    
    arc4.ProcessData((CryptoPP::byte*)message.data(), (CryptoPP::byte*)message.data(), message.size());

    std::cout << "Encrypted message: " << message << "\n";

    // Reset
    arc4.SetKey((CryptoPP::byte*)key.data(), key.size());
    arc4.ProcessData((CryptoPP::byte*)message.data(), (CryptoPP::byte*)message.data(), message.size());

    std::cout << "Decrypted message: " << message << "\n";
    

    return 0;

}