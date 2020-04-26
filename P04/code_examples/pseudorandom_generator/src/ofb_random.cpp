/*
 * source: https://stackoverflow.com/questions/28515793/how-to-regenerate-values-in-autoseededx917rng
 */

#include <iostream>

#include <cryptopp/cryptlib.h>
#include <cryptopp/secblock.h>
#include <cryptopp/osrng.h>
#include <cryptopp/modes.h>
#include <cryptopp/hex.h>
#include <cryptopp/integer.h>

int main(void) {

    CryptoPP::SecByteBlock seed(32 + 16);
    CryptoPP::OS_GenerateRandomBlock(false, seed, seed.size());

    CryptoPP::OFB_Mode<CryptoPP::AES>::Encryption prng;
    prng.SetKeyWithIV(seed, 32, seed + 32, 16);

    for (unsigned int i = 0; i < 10; ++i){

        // CryptoPP::SecByteBlock t(16);
        // prng.GenerateBlock(t, t.size());

        std::cout << i << " " << std::hex << CryptoPP::Integer(prng, 64) << std::dec << "\n";

        // std::string s;
        // CryptoPP::HexEncoder hex(new CryptoPP::StringSink(s));

        // hex.Put(t, t.size());
        // hex.MessageEnd();

        // std::cout << "Random: " << s << "\n";

    }

    return 0;

}